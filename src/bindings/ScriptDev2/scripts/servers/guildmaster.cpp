#include "precompiled.h"

extern DatabaseMysql SD2Database;

#define MSG_GOSSIP_TELE          "Teleport to GuildHouse"
#define MSG_GOSSIP_BUY           "Buy GuildHouse (1000 gold)"
#define MSG_GOSSIP_SELL          "Sell GuildHouse (500 gold)"
#define MSG_GOSSIP_NEXTPAGE      "Next page -->"
#define MSG_INCOMBAT             "You are in combat!"
#define MSG_NOGUILDHOUSE         "Your guild does not have a GuildHouse, loosers!"
#define MSG_NOFREEGH             "Sorry, all guildhouses are occupied."
#define MSG_ALREADYHAVEGH        "Sorry, but you already have GuildHouse (%s)."
#define MSG_NOTENOUGHMONEY       "Not enough money. You need %u gold to buy GuildHouse."
#define MSG_GHOCCUPIED           "Sorry, this GuildHouse already occupied."
#define MSG_CONGRATULATIONS      "Congratulations! You have GuildHouse now."
#define MSG_SOLD                 "You have sold GuildHouse. Here is your %u gold."
#define MSG_NOTINGUILD           "You are not in guild. Go find good one."

#define CODE_SELL                "SELL"
#define MSG_CODEBOX_SELL         "Type \"" CODE_SELL "\" (with CAPS!!!) in next window if you wanna sell your guildhouse."

#define OFFSET_GH_ID_TO_ACTION   1500
#define OFFSET_SHOWBUY_FROM      10000

#define ACTION_TELE              1001
#define ACTION_SHOW_BUYLIST      1002  //deprecated. Use (OFFSET_SHOWBUY_FROM + 0) instead
#define ACTION_SELL_GUILDHOUSE   1003

#define ICON_GOSSIP_BALOON       0
#define ICON_GOSSIP_WING         2
#define ICON_GOSSIP_BOOK         3
#define ICON_GOSSIP_WHEEL1       4
#define ICON_GOSSIP_WHEEL2       5
#define ICON_GOSSIP_GOLD         6
#define ICON_GOSSIP_BALOONDOTS   7
#define ICON_GOSSIP_TABARD       8
#define ICON_GOSSIP_XSWORDS      9

#define COST_GH_BUY              10000000  //1000 g.
#define COST_GH_SELL             5000000   //500 g.

#define GOSSIP_COUNT_MAX         10

bool isPlayerGuildLeader(Player *player)
{
	return (player->GetRank() == 0) && (player->GetGuildId() != 0);
}

bool getGuildHouseCoords(uint32 guildId, float &x, float &y, float &z, uint32 &map)
{
	if (guildId == 0)
	{
		//if player has no guild
		return false;
	}

	QueryResult *result;
	result = SD2Database.PQuery("SELECT `x`, `y`, `z`, `map` FROM `guildhouses` WHERE `guildId` = %u", guildId);
	if(result)
	{
		Field *fields = result->Fetch();
		x = fields[0].GetFloat();
		y = fields[1].GetFloat();
		z = fields[2].GetFloat();
		map = fields[3].GetUInt32();
		delete result;
		return true;
	}
	return false;
}

void teleportPlayerToGuildHouse(Player *player, Creature *_creature)
{
	if (player->GetGuildId() == 0)
	{
		//if player has no guild
		_creature->MonsterWhisper(MSG_NOTINGUILD, player->GetGUID());
		return;
	}

	if (!player->getAttackers().empty())
	{
		//if player in combat
		_creature->MonsterSay(MSG_INCOMBAT, LANG_UNIVERSAL, player->GetGUID());
		return;
	}

	float x, y, z;
	uint32 map;

	if (getGuildHouseCoords(player->GetGuildId(), x, y, z, map))
	{
		//teleport player to the specified location
		player->TeleportTo(map, x, y, z, 0.0f);
	}
	else
		_creature->MonsterWhisper(MSG_NOGUILDHOUSE, player->GetGUID());

}

bool showBuyList(Player *player, Creature *_creature, uint32 showFromId = 0)
{
	//show not occupied guildhouses

	QueryResult *result;
	result = SD2Database.PQuery("SELECT `id`, `comment` FROM `guildhouses` WHERE `guildId` = 0 AND `id` > %u ORDER BY `id` ASC LIMIT %u",
		showFromId, GOSSIP_COUNT_MAX);

	if (result)
	{
		uint32 guildhouseId = 0;
		std::string comment = "";
		do
		{

			Field *fields = result->Fetch();

			guildhouseId = fields[0].GetInt32();
			comment = fields[1].GetString();
			
			//send comment as a gossip item
			//transmit guildhouseId in Action variable
			player->ADD_GOSSIP_ITEM(ICON_GOSSIP_TABARD, comment, GOSSIP_SENDER_MAIN,
				guildhouseId + OFFSET_GH_ID_TO_ACTION);

		} while (result->NextRow());

		if (result->GetRowCount() == GOSSIP_COUNT_MAX)
		{
			//assume that we have additional page
			//add link to next GOSSIP_COUNT_MAX items
			player->ADD_GOSSIP_ITEM(ICON_GOSSIP_BALOONDOTS, MSG_GOSSIP_NEXTPAGE, GOSSIP_SENDER_MAIN, 
				guildhouseId + OFFSET_SHOWBUY_FROM);
		}

		delete result;

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());

		return true;
	} else
	{
		if (showFromId = 0)
		{
			//all guildhouses are occupied
			_creature->MonsterWhisper(MSG_NOFREEGH, player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
		} else
		{
			//this condition occurs when COUNT(guildhouses) % GOSSIP_COUNT_MAX == 0
			//just show GHs from beginning
			showBuyList(player, _creature, 0);
		}
	}

	return false;
}

bool isPlayerHasGuildhouse(Player *player, Creature *_creature, bool whisper = false)
{

	QueryResult *result;

	result = SD2Database.PQuery("SELECT `comment` FROM `guildhouses` WHERE `guildId` = %u",
		player->GetGuildId());

	if (result)
	{
		if (whisper)
		{
			//whisper to player "already have etc..."
			Field *fields = result->Fetch();
			char msg[100];
			sprintf(msg, MSG_ALREADYHAVEGH, fields[0].GetString());
			_creature->MonsterWhisper(msg, player->GetGUID());
		}

		delete result;
		return true;
	}
	return false;

}

void buyGuildhouse(Player *player, Creature *_creature, uint32 guildhouseId)
{
	if (player->GetMoney() < COST_GH_BUY)
	{
		//show how much money player need to buy GH (in gold)
		char msg[100];
		sprintf(msg, MSG_NOTENOUGHMONEY, COST_GH_BUY / 10000);
		_creature->MonsterWhisper(msg, player->GetGUID());
		return;
	}

	if (isPlayerHasGuildhouse(player, _creature, true))
	{
		//player already have GH
		return;
	}

	QueryResult *result;

	//check if somebody already occupied this GH
	result = SD2Database.PQuery("SELECT `id` FROM `guildhouses` WHERE `id` = %u AND `guildId` <> 0",
		guildhouseId);

	if (result)
	{
		delete result;
		_creature->MonsterWhisper(MSG_GHOCCUPIED, player->GetGUID());
		return;
	}

	//update DB
	result = SD2Database.PQuery("UPDATE `guildhouses` SET `guildId` = %u WHERE `id` = %u",
		player->GetGuildId(), guildhouseId);

	if (result)
		delete result;

	player->ModifyMoney(-COST_GH_BUY);
	_creature->MonsterSay(MSG_CONGRATULATIONS, LANG_UNIVERSAL, player->GetGUID());
	
}

void sellGuildhouse(Player *player, Creature *_creature)
{
	if (isPlayerHasGuildhouse(player, _creature))
	{
		QueryResult *result;

		result = SD2Database.PQuery("UPDATE `guildhouses` SET `guildId` = 0 WHERE `guildId` = %u",
		player->GetGuildId());
		
		if (result)
			delete result;

		player->ModifyMoney(COST_GH_SELL);

		//display message e.g. "here your money etc."
		char msg[100];
		sprintf(msg, MSG_SOLD, COST_GH_SELL / 10000);
		_creature->MonsterWhisper(msg, player->GetGUID());
	}
}

bool GossipHello_guildmaster(Player *player, Creature *_creature)
{
	player->ADD_GOSSIP_ITEM(ICON_GOSSIP_BALOON, MSG_GOSSIP_TELE, 
		GOSSIP_SENDER_MAIN, ACTION_TELE);

	if (isPlayerGuildLeader(player))
	{
		//show additional menu for guild leader
		player->ADD_GOSSIP_ITEM(ICON_GOSSIP_GOLD, MSG_GOSSIP_BUY,
			GOSSIP_SENDER_MAIN, ACTION_SHOW_BUYLIST);
		if (isPlayerHasGuildhouse(player, _creature))
		{
			//and additional for guildhouse owner
			player->PlayerTalkClass->GetGossipMenu().AddMenuItem(ICON_GOSSIP_GOLD, MSG_GOSSIP_SELL, GOSSIP_SENDER_MAIN, ACTION_SELL_GUILDHOUSE, MSG_CODEBOX_SELL, 0, true);
		}
	}
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
	return true;
}


bool GossipSelect_guildmaster(Player *player, Creature *_creature, uint32 sender, uint32 action )
{
	if (sender != GOSSIP_SENDER_MAIN)
		return false;

	switch (action)
	{
		case ACTION_TELE:
			//teleport player to GH
			player->CLOSE_GOSSIP_MENU();
			teleportPlayerToGuildHouse(player, _creature);
			break;
		case ACTION_SHOW_BUYLIST:
			//show list of GHs which currently not occupied
			showBuyList(player, _creature);
			break;
		default:
			if (action > OFFSET_SHOWBUY_FROM)
			{
				showBuyList(player, _creature, action - OFFSET_SHOWBUY_FROM);
			} else if (action > OFFSET_GH_ID_TO_ACTION)
			{
				//player clicked on buy list
				player->CLOSE_GOSSIP_MENU();
				//get guildhouseId from action
				//guildhouseId = action - OFFSET_GH_ID_TO_ACTION
				buyGuildhouse(player, _creature, action - OFFSET_GH_ID_TO_ACTION);
			}
			break;
	}
	
	return true;
}

bool GossipSelectWithCode_guildmaster( Player *player, Creature *_creature,
									  uint32 sender, uint32 action, const char* sCode )
{
    if(sender == GOSSIP_SENDER_MAIN)
    {
        if(action == ACTION_SELL_GUILDHOUSE)
        {
			int i = -1;
			try
			{
				//compare code
				if (strlen(sCode) + 1 == sizeof CODE_SELL)
					i = strcmp(CODE_SELL, sCode);

			} catch(char *str) {error_db_log(str);}

			if (i == 0)
			{
				//right code
				sellGuildhouse(player, _creature);
			}
			player->CLOSE_GOSSIP_MENU();
			return true;
		}
	}
	return false;
}


void AddSC_guildmaster()
{
	Script *newscript;
	newscript = new Script;
	newscript->Name = "guildmaster";
	newscript->pGossipHello = &GossipHello_guildmaster;
	newscript->pGossipSelect = &GossipSelect_guildmaster;
	newscript->pGossipSelectWithCode =  &GossipSelectWithCode_guildmaster;
	newscript->RegisterSelf();
}
/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_PLAYER_H
#define SC_PLAYER_H

#include "Player.h"
#include "GossipDef.h"
#include "QuestDef.h"

// Gossip Item Text
#define GOSSIP_TEXT_BROWSE_GOODS        -1999831
#define GOSSIP_TEXT_TRAIN               "Train me!"

#define GOSSIP_TEXT_BANK                "The bank"
#define GOSSIP_TEXT_IRONFORGE_BANK      "The bank of Ironforge"
#define GOSSIP_TEXT_STORMWIND_BANK      "The bank of Stormwind"
#define GOSSIP_TEXT_STORMWIND_HARBOR    "Stormwind Harbor"
#define GOSSIP_TEXT_WINDRIDER           "The wind rider master"
#define GOSSIP_TEXT_GRYPHON             "The gryphon master"
#define GOSSIP_TEXT_BATHANDLER          "The bat handler"
#define GOSSIP_TEXT_HIPPOGRYPH          "The hippogryph master"
#define GOSSIP_TEXT_ZEPPLINMASTER       "The zeppelin master"
#define GOSSIP_TEXT_DEEPRUNTRAM         "The Deeprun Tram"
#define GOSSIP_TEXT_FERRY               "The Rut'theran Ferry"
#define GOSSIP_TEXT_FLIGHTMASTER        "The flight master"
#define GOSSIP_TEXT_AUCTIONHOUSE        "The auction house"
#define GOSSIP_TEXT_GUILDMASTER         "The guild master"
#define GOSSIP_TEXT_LOCKSMITH           "Stormwind Locksmith"
#define GOSSIP_TEXT_INN                 "The inn"
#define GOSSIP_TEXT_MAILBOX             -1999830
#define GOSSIP_TEXT_STABLEMASTER        "The stable master"
#define GOSSIP_TEXT_WEAPONMASTER        "The weapon master"
#define GOSSIP_TEXT_OFFICERS            "The officers' lounge"
#define GOSSIP_TEXT_BATTLEMASTER        "The battlemaster"
#define GOSSIP_TEXT_BARBER              "Barber"
#define GOSSIP_TEXT_CLASSTRAINER        "A class trainer"
#define GOSSIP_TEXT_PROFTRAINER         "A profession trainer"
#define GOSSIP_TEXT_LEXICON             "Lexicon of Power"

#define GOSSIP_TEXT_ALTERACVALLEY       "Alterac Valley"
#define GOSSIP_TEXT_ARATHIBASIN         "Arathi Basin"
#define GOSSIP_TEXT_WARSONGULCH         "Warsong Gulch"
#define GOSSIP_TEXT_ARENA               "Arena"
#define GOSSIP_TEXT_EYEOFTHESTORM       "Eye of The Storm"
#define GOSSIP_TEXT_STRANDOFANCIENT     "Strand of the Ancients"

#define GOSSIP_TEXT_DEATH_KNIGHT        "Death Knight"
#define GOSSIP_TEXT_DRUID               "Druid"
#define GOSSIP_TEXT_HUNTER              "Hunter"
#define GOSSIP_TEXT_PRIEST              "Priest"
#define GOSSIP_TEXT_ROGUE               "Rogue"
#define GOSSIP_TEXT_WARRIOR             "Warrior"
#define GOSSIP_TEXT_PALADIN             "Paladin"
#define GOSSIP_TEXT_SHAMAN              "Shaman"
#define GOSSIP_TEXT_MAGE                "Mage"
#define GOSSIP_TEXT_WARLOCK             "Warlock"
#define GOSSIP_TEXT_DEATH_KNIGHT        "Death Knight"

#define GOSSIP_TEXT_ALCHEMY             "Alchemy"
#define GOSSIP_TEXT_BLACKSMITHING       "Blacksmithing"
#define GOSSIP_TEXT_COOKING             "Cooking"
#define GOSSIP_TEXT_ENCHANTING          "Enchanting"
#define GOSSIP_TEXT_ENGINEERING         "Engineering"
#define GOSSIP_TEXT_FIRSTAID            "First Aid"
#define GOSSIP_TEXT_HERBALISM           "Herbalism"
#define GOSSIP_TEXT_LEATHERWORKING      "Leatherworking"
#define GOSSIP_TEXT_TAILORING           "Tailoring"
#define GOSSIP_TEXT_MINING              "Mining"
#define GOSSIP_TEXT_FISHING             "Fishing"
#define GOSSIP_TEXT_SKINNING            "Skinning"
#define GOSSIP_TEXT_JEWELCRAFTING       "Jewelcrafting"
#define GOSSIP_TEXT_RUNEFORGING         "Runeforging"
#define GOSSIP_TEXT_INSCRIPTION         "Inscription"

// Skill defines

#define TRADESKILL_ALCHEMY                  1
#define TRADESKILL_BLACKSMITHING            2
#define TRADESKILL_COOKING                  3
#define TRADESKILL_ENCHANTING               4
#define TRADESKILL_ENGINEERING              5
#define TRADESKILL_FIRSTAID                 6
#define TRADESKILL_HERBALISM                7
#define TRADESKILL_LEATHERWORKING           8
#define TRADESKILL_POISONS                  9
#define TRADESKILL_TAILORING                10
#define TRADESKILL_MINING                   11
#define TRADESKILL_FISHING                  12
#define TRADESKILL_SKINNING                 13
#define TRADESKILL_JEWLCRAFTING             14
#define TRADESKILL_INSCRIPTION              15
#define TRADESKILL_RUNEFORGING              16

#define TRADESKILL_LEVEL_NONE               0
#define TRADESKILL_LEVEL_APPRENTICE         1
#define TRADESKILL_LEVEL_JOURNEYMAN         2
#define TRADESKILL_LEVEL_EXPERT             3
#define TRADESKILL_LEVEL_ARTISAN            4
#define TRADESKILL_LEVEL_MASTER             5
#define TRADESKILL_LEVEL_GRAND_MASTER       6

// Gossip defines

#define GOSSIP_ACTION_TRADE                 1
#define GOSSIP_ACTION_TRAIN                 2
#define GOSSIP_ACTION_TAXI                  3
#define GOSSIP_ACTION_GUILD                 4
#define GOSSIP_ACTION_BATTLE                5
#define GOSSIP_ACTION_BANK                  6
#define GOSSIP_ACTION_INN                   7
#define GOSSIP_ACTION_HEAL                  8
#define GOSSIP_ACTION_TABARD                9
#define GOSSIP_ACTION_AUCTION               10
#define GOSSIP_ACTION_INN_INFO              11
#define GOSSIP_ACTION_UNLEARN               12
#define GOSSIP_ACTION_INFO_DEF              1000

#define GOSSIP_SENDER_MAIN                  1
#define GOSSIP_SENDER_INN_INFO              2
#define GOSSIP_SENDER_INFO                  3
#define GOSSIP_SENDER_SEC_PROFTRAIN         4
#define GOSSIP_SENDER_SEC_CLASSTRAIN        5
#define GOSSIP_SENDER_SEC_BATTLEINFO        6
#define GOSSIP_SENDER_SEC_BANK              7
#define GOSSIP_SENDER_SEC_INN               8
#define GOSSIP_SENDER_SEC_MAILBOX           9
#define GOSSIP_SENDER_SEC_STABLEMASTER      10

#define DEFAULT_GOSSIP_MESSAGE              0xffffff

extern uint32 GetSkillLevel(Player* pPlayer,uint32 skill);

// Defined fuctions to use with player.

// This fuction add's a menu item,
// a - Icon Id
// b - Text
// c - Sender(this is to identify the current Menu with this item)
// d - Action (identifys this Menu Item)
// e - Text to be displayed in pop up box
// f - Money value in pop up box
#define ADD_GOSSIP_ITEM(a,b,c,d)   PlayerTalkClass->GetGossipMenu().AddMenuItem(a,b,c,d,"",0)
#define ADD_GOSSIP_ITEM_EXTENDED(a,b,c,d,e,f,g)   PlayerTalkClass->GetGossipMenu().AddMenuItem(a,b,c,d,e,f,g)

// This fuction Sends the current menu to show to client, a - NPCTEXTID(uint32) , b - npc guid(uint64)
#define SEND_GOSSIP_MENU(a,b)      PlayerTalkClass->SendGossipMenu(a,b)

// This fuction shows POI(point of interest) to client.
// a - position X
// b - position Y
// c - Icon Id
// d - Flags
// e - Data
// f - Location Name
#define SEND_POI(a,b,c,d,e,f)      PlayerTalkClass->SendPointOfInterest(a,b,c,d,e,f)

// Closes the Menu
#define CLOSE_GOSSIP_MENU()        PlayerTalkClass->CloseGossip()

// Fuction to tell to client the details
// a - quest object
// b - npc guid(uint64)
// c - Activate accept(bool)
#define SEND_QUEST_DETAILS(a,b,c)  PlayerTalkClass->SendQuestDetails(a,b,c)

// Fuction to tell to client the requested items to complete quest
// a - quest object
// b - npc guid(uint64)
// c - Iscompletable(bool)
// d - close at cancel(bool) - in case single incomplite ques
#define SEND_REQUESTEDITEMS(a,b,c,d) PlayerTalkClass->SendRequestedItems(a,b,c,d)

// Fuctions to send NPC lists, a - is always the npc guid(uint64)
#define SEND_VENDORLIST(a)         GetSession()->SendListInventory(a)
#define SEND_TRAINERLIST(a)        GetSession()->SendTrainerList(a)
#define SEND_BANKERLIST(a)         GetSession()->SendShowBank(a)
#define SEND_TABARDLIST(a)         GetSession()->SendTabardVendorActivate(a)
#define SEND_AUCTIONLIST(a)        GetSession()->SendAuctionHello(a)
#define SEND_TAXILIST(a)           GetSession()->SendTaxiStatus(a)

// Ressurect's the player if is dead.
#define SEND_SPRESURRECT()         GetSession()->SendSpiritResurrect()

// Get the player's honor rank.
#define GET_HONORRANK()            GetHonorRank()
// -----------------------------------

// defined fuctions to use with Creature

#define QUEST_DIALOG_STATUS(a,b,c)   GetSession()->getDialogStatus(a,b,c)
#endif

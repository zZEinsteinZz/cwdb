/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Thunder_Bluff
SD%Complete: 100
SDComment: Quest support: 925, 11129
SDCategory: Thunder Bluff
EndScriptData */

#include "precompiled.h"

/*#####
# npc_cairne_bloodhoof
######*/

#define SPELL_BERSERKER_CHARGE  16636
#define SPELL_CLEAVE            16044
#define SPELL_MORTAL_STRIKE     16856
#define SPELL_THUNDERCLAP       23931
#define SPELL_UPPERCUT          22916

//TODO: verify abilities/timers
struct MANGOS_DLL_DECL npc_cairne_bloodhoofAI : public ScriptedAI
{
    npc_cairne_bloodhoofAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 BerserkerCharge_Timer;
    uint32 Cleave_Timer;
    uint32 MortalStrike_Timer;
    uint32 Thunderclap_Timer;
    uint32 Uppercut_Timer;

    void Reset()
    {
        BerserkerCharge_Timer = 30000;
        Cleave_Timer = 5000;
        MortalStrike_Timer = 10000;
        Thunderclap_Timer = 15000;
        Uppercut_Timer = 10000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (BerserkerCharge_Timer < diff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)
                DoCast(target,SPELL_BERSERKER_CHARGE);
            BerserkerCharge_Timer = 25000;
        }else BerserkerCharge_Timer -= diff;

        if (Uppercut_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_UPPERCUT);
            Uppercut_Timer = 20000;
        }else Uppercut_Timer -= diff;

        if (Thunderclap_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_THUNDERCLAP);
            Thunderclap_Timer = 15000;
        }else Thunderclap_Timer -= diff;

        if (MortalStrike_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MORTAL_STRIKE);
            MortalStrike_Timer = 15000;
        }else MortalStrike_Timer -= diff;

        if (Cleave_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_CLEAVE);
            Cleave_Timer = 7000;
        }else Cleave_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_npc_cairne_bloodhoof(Creature* pCreature)
{
    return new npc_cairne_bloodhoofAI(pCreature);
}

bool GossipHello_npc_cairne_bloodhoof(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(925) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(0, "I know this is rather silly but a young ward who is a bit shy would like your hoofprint.", GOSSIP_SENDER_MAIN, GOSSIP_SENDER_INFO);

    pPlayer->SEND_GOSSIP_MENU(7013, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_cairne_bloodhoof(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_SENDER_INFO)
    {
        pPlayer->CastSpell(pPlayer, 23123, false);
        pPlayer->SEND_GOSSIP_MENU(7014, pCreature->GetGUID());
    }
    return true;
}

/*#####
# npc_kyle_frenzied
######*/

struct MANGOS_DLL_DECL npc_kyle_frenziedAI : public ScriptedAI
{
    npc_kyle_frenziedAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    int STATE;
    uint32 wait;
    uint64 player;

    void Reset()
    {
        STATE = 0;
        m_creature->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
        m_creature->GetMotionMaster()->Initialize();
    }
    void Aggro(Unit* who){}

    void SpellHit(Unit *caster, const SpellEntry* spell)
    {    // we can feed him without any quest
        if(spell->Id == 42222 && caster->GetTypeId() == TYPEID_PLAYER && ((Player*)caster)->GetTeam() == HORDE)
        {
            STATE = 1;
            player = caster->GetGUID();
            float x, y, z, z2;
            caster->GetPosition(x, y, z);
            x = x + 3.7*cos(caster->GetOrientation());
            y = y + 3.7*sin(caster->GetOrientation());
            z2 = m_creature->GetBaseMap()->GetHeight(x,y,z,false);
            z = (z2 <= INVALID_HEIGHT) ? z : z2;
            m_creature->SetDefaultMovementType(IDLE_MOTION_TYPE);        //there is other way to stop waypoint movement?
            m_creature->GetMotionMaster()->Initialize();
            m_creature->RemoveUnitMovementFlag(MOVEMENTFLAG_WALK_MODE);
            m_creature->GetMotionMaster()->MovePoint(0,x, y, z);
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(type == POINT_MOTION_TYPE)
        {    
            switch(STATE)
            {
            case 1:
                {
                Unit *plr = Unit::GetUnit((*m_creature),player);
                if(plr)
                m_creature->SetOrientation(m_creature->GetAngle(plr));
                m_creature->HandleEmoteCommand(EMOTE_STATE_USESTANDING);    //eat
                WorldPacket data;
                m_creature->BuildHeartBeatMsg(&data);
                m_creature->SendMessageToSet(&data,true);
                wait = 3000;
                STATE = 2;
                break;
                }
            case 4:
                m_creature->setDeathState(JUST_DIED);
                m_creature->Respawn();
                break;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!STATE || STATE == 4)
            return;
        if(wait < diff)
        {
            switch(STATE)
            {
            case 2: 
                STATE = 3; wait = 7000;
                m_creature->UpdateEntry(23622,HORDE);
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_DANCE);
                break;
            case 3:
                STATE = 4;    //go home
                Unit *plr = Unit::GetUnit((*m_creature),player);
                    if(plr && ((Player*)plr)->GetQuestStatus(11129) == QUEST_STATUS_INCOMPLETE)
                        ((Player*)plr)->AreaExploredOrEventHappens(11129);
                float x, y, z, z2, angle;
                angle = m_creature->GetAngle(-2146, -430);
                m_creature->GetPosition(x,y,z);
                x = x + 40*cos(angle);
                y = y + 40*sin(angle);
                z2 = m_creature->GetBaseMap()->GetHeight(x,y,MAX_HEIGHT,false);
                z = (z2 <= INVALID_HEIGHT) ? z : z2;
                m_creature->GetMotionMaster()->MovePoint(0,x,y,z);
                break;
            }
        }else wait -= diff;
    }
};

CreatureAI* GetAI_npc_kyle_frenzied(Creature* pCreature)
{
    return new npc_kyle_frenziedAI(pCreature);
}

void AddSC_thunder_bluff()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_cairne_bloodhoof";
    newscript->GetAI = &GetAI_npc_cairne_bloodhoof;
    newscript->pGossipHello = &GossipHello_npc_cairne_bloodhoof;
    newscript->pGossipSelect = &GossipSelect_npc_cairne_bloodhoof;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_kyle_frenzied";
    newscript->GetAI = &GetAI_npc_kyle_frenzied;
    newscript->RegisterSelf();
}

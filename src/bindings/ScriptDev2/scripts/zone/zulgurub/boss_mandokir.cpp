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
SDName: Boss_Mandokir
SD%Complete: 90
SDComment: Ohgan function needs improvements.
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "def_zulgurub.h"

#define SAY_AGGRO               -1309015
#define SAY_DING_KILL           -1309016
#define SAY_GRATS_JINDO         -1309017
#define SAY_WATCH               -1309018
#define SAY_WATCH_WHISPER       -1309019                    //is this text for real? easter egg?

#define SPELL_CHARGE            24315
#define SPELL_CLEAVE            20691
#define SPELL_FEAR              29321
#define SPELL_WHIRLWIND         24236
#define SPELL_MORTAL_STRIKE     24573
#define SPELL_ENRAGE            23537
#define SPELL_WATCH             24314
#define SPELL_LEVEL_UP          24312

//Ohgans Spells
#define SPELL_SUNDERARMOR       24317

struct MANGOS_DLL_DECL boss_mandokirAI : public ScriptedAI
{
    boss_mandokirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 KillCount;
    uint32 Watch_Timer;
    uint32 TargetInRange;
    uint32 Cleave_Timer;
    uint32 Whirlwind_Timer;
    uint32 Fear_Timer;
    uint32 MortalStrike_Timer;
    uint32 Check_Timer;
    float targetX;
    float targetY;
    float targetZ;

    bool endWatch;
    bool someWatched;
    bool RaptorDead;
    bool CombatStart;

    uint64 WatchTarget;

    void Reset()
    {
        KillCount = 0;
        Watch_Timer = 33000;
        Cleave_Timer = 7000;
        Whirlwind_Timer = 20000;
        Fear_Timer = 1000;
        MortalStrike_Timer = 1000;
        Check_Timer = 1000;

        targetX = 0.0;
        targetY = 0.0;
        targetZ = 0.0;
        TargetInRange = 0;

        WatchTarget = 0;

        someWatched = false;
        endWatch = false;
        RaptorDead = false;
        CombatStart = false;

        DoCast(m_creature, 23243);
    }

    void KilledUnit(Unit* victim)
    {
        if (victim->GetTypeId() == TYPEID_PLAYER)
        {
            ++KillCount;

            if (KillCount == 3)
            {
                DoScriptText(SAY_DING_KILL, m_creature);

                if (pInstance)
                {
                    uint64 JindoGUID = pInstance->GetData64(DATA_JINDO);
                    if (JindoGUID)
                    {
                        if (Unit* jTemp = Unit::GetUnit(*m_creature,JindoGUID))
                        {
                            if (jTemp->isAlive())
                                DoScriptText(SAY_GRATS_JINDO, jTemp);
                        }
                    }
                }

                DoCast(m_creature, SPELL_LEVEL_UP, true);
                KillCount = 0;
            }
        }
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (!CombatStart)
        {
            //At combat Start Mandokir is mounted so we must unmount it first
            m_creature->Unmount();

            //And summon his raptor
            m_creature->SummonCreature(14988, m_creature->getVictim()->GetPositionX(), m_creature->getVictim()->GetPositionY(), m_creature->getVictim()->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 35000);
            CombatStart = true;
        }

        if (Watch_Timer < diff)                             //Every 20 Sec Mandokir will check this
        {
            if (WatchTarget)                                //If someone is watched and If the Position of the watched target is different from the one stored, or are attacking, mandokir will charge him
            {
                Unit* pUnit = Unit::GetUnit(*m_creature, WatchTarget);

                if (pUnit && (
                    targetX != pUnit->GetPositionX() ||
                    targetY != pUnit->GetPositionY() ||
                    targetZ != pUnit->GetPositionZ() ||
                    pUnit->isInCombat()))
                {
                    if (m_creature->IsWithinDistInMap(pUnit, ATTACK_DISTANCE))
                    {
                        DoCast(pUnit,24316);
                    }
                    else
                    {
                        DoCast(pUnit,SPELL_CHARGE);
                        m_creature->SendMonsterMove(pUnit->GetPositionX(), pUnit->GetPositionY(), pUnit->GetPositionZ(), 0, true,1);
                        AttackStart(pUnit);
                    }
                }
            }
            someWatched = false;
            Watch_Timer = 20000;
        }else Watch_Timer -= diff;

        if ((Watch_Timer < 8000) && !someWatched)           //8 sec(cast time + expire time) before the check for the watch effect mandokir will cast watch debuff on a random target
        {
            if (Unit* p = SelectUnit(SELECT_TARGET_RANDOM,0))
            {
                DoScriptText(SAY_WATCH, m_creature, p);
                DoCast(p, SPELL_WATCH);
                WatchTarget = p->GetGUID();
                someWatched = true;
                endWatch = true;
            }
        }

        if ((Watch_Timer < 1000) && endWatch)               //1 sec before the debuf expire, store the target position
        {
            Unit* pUnit = Unit::GetUnit(*m_creature, WatchTarget);
            if (pUnit)
            {
                targetX = pUnit->GetPositionX();
                targetY = pUnit->GetPositionY();
                targetZ = pUnit->GetPositionZ();
            }
            endWatch = false;
        }

        if (!someWatched)
        {
            //Cleave
            if (Cleave_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_CLEAVE);
                Cleave_Timer = 7000;
            }else Cleave_Timer -= diff;

            //Whirlwind
            if (Whirlwind_Timer < diff)
            {
                DoCast(m_creature,SPELL_WHIRLWIND);
                Whirlwind_Timer = 18000;
            }else Whirlwind_Timer -= diff;

            //If more then 3 targets in melee range mandokir will cast fear
            if (Fear_Timer < diff)
            {
                TargetInRange = 0;

                std::list<HostilReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
                for(; i != m_creature->getThreatManager().getThreatList().end(); ++i)
                {
                    Unit* pUnit = Unit::GetUnit(*m_creature, (*i)->getUnitGuid());
                    if (pUnit && m_creature->IsWithinDistInMap(pUnit, ATTACK_DISTANCE))
                        TargetInRange++;
                }

                if (TargetInRange > 3)
                    DoCast(m_creature->getVictim(),SPELL_FEAR);

                Fear_Timer = 4000;
            }else Fear_Timer -=diff;

            //Mortal Strike if target below 50% hp
            if (m_creature->getVictim()->GetHealth() < m_creature->getVictim()->GetMaxHealth()*0.5)
            {
                if (MortalStrike_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_MORTAL_STRIKE);
                    MortalStrike_Timer = 15000;
                }else MortalStrike_Timer -= diff;
            }
        }

        //Checking if Ohgan is dead. If yes Mandokir will enrage.
        if (Check_Timer < diff)
        {
            if (pInstance)
            {
                if (pInstance->GetData(DATA_OHGANISDEAD))
                {
                    if (!RaptorDead)
                    {
                        DoCast(m_creature, SPELL_ENRAGE);
                        RaptorDead = true;
                    }
                }
            }

            Check_Timer = 1000;
        }else Check_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

//Ohgan
struct MANGOS_DLL_DECL mob_ohganAI : public ScriptedAI
{
    mob_ohganAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 SunderArmor_Timer;

    void Reset()
    {
        SunderArmor_Timer = 5000;
    }

    void JustDied(Unit* Killer)
    {
        if (pInstance)
            pInstance->SetData(DATA_OHGAN_DEATH, 0);
    }

    void UpdateAI (const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //SunderArmor_Timer
        if (SunderArmor_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_SUNDERARMOR);
            SunderArmor_Timer = 10000 + rand()%5000;
        }else SunderArmor_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_mandokir(Creature* pCreature)
{
    return new boss_mandokirAI(pCreature);
}

CreatureAI* GetAI_mob_ohgan(Creature* pCreature)
{
    return new mob_ohganAI(pCreature);
}

void AddSC_boss_mandokir()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_mandokir";
    newscript->GetAI = &GetAI_boss_mandokir;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ohgan";
    newscript->GetAI = &GetAI_mob_ohgan;
    newscript->RegisterSelf();
}

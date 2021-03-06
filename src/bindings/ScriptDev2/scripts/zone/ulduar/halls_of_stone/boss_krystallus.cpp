/* Script Data Start
SDName: Boss krystallus
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_krystallus' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_BOULDER_TOSS                             50843
#define SPELL_BOULDER_TOSS_2                           59742
#define SPELL_GROUND_SPIKE                             59750
#define SPELL_SHATTER                                  50810
#define SPELL_SHATTER_2                                61546
#define SPELL_STOMP                                    48131
#define SPELL_STOMP_2                                  59744

//Yell
#define SAY_AGGRO                                   -1999701
#define SAY_KILL                                    -1999700
#define SAY_DEATH                                   -1999699
#define SAY_SHATTER                                 -1999698

struct MANGOS_DLL_DECL boss_krystallusAI : public ScriptedAI
{
    boss_krystallusAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() {}
    void Aggro(Unit* who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }
    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}
    void UpdateAI(const uint32 diff) 
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;
                
        DoMeleeAttackIfReady();    
    }
    void JustDied(Unit* killer)  
    {
        DoScriptText(SAY_DEATH, m_creature);
    }
    void KilledUnit(Unit *victim)
    {
        if(victim == m_creature)
            return;
        DoScriptText(SAY_KILL, m_creature);
    }
};

CreatureAI* GetAI_boss_krystallus(Creature* pCreature)
{
    return new boss_krystallusAI(pCreature);
}

void AddSC_boss_krystallus()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_krystallus";
    newscript->GetAI = GetAI_boss_krystallus;
    newscript->RegisterSelf();
}

/* Script Data Start
SDName: Boss trollgore
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_trollgore' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spell
#define SPELL_INFECTED_WOUND                          49637
#define SPELL_CRUSH                                   49639
#define SPELL_CORPSE_EXPLODE_N                        49555
#define SPELL_CONSUME_N                               49380

//Spell Heroic
#define SPELL_CORPSE_EXPLODE_H                        59807
#define SPELL_CONSUME_H                               59803

//Yell
#define SAY_AGGRO                                  -1999847
#define SAY_KILL                                   -1999847
#define SAY_CONSUME                                -1999847
#define SAY_EXPLODE                                -1999847
#define SAY_DEATH                                  -1999847

struct MANGOS_DLL_DECL boss_trollgoreAI : public ScriptedAI
{
    boss_trollgoreAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

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
        if (victim == m_creature)
            return;
        DoScriptText(SAY_KILL, m_creature);
    }
};

CreatureAI* GetAI_boss_trollgore(Creature* pCreature)
{
    return new boss_trollgoreAI(pCreature);
}

void AddSC_boss_trollgore()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_trollgore";
    newscript->GetAI = GetAI_boss_trollgore;
    newscript->RegisterSelf();
}

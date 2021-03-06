/* Script Data Start
SDName: Boss palehoof
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_palehoof' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_ARCING_SMASH                          48260
#define SPELL_IMPALE                                48261
#define SPELL_WITHERING_ROAR                        48256

//Yell
#define SAY_AGGRO                                -1999660
#define SAY_SLAY_1                               -1999659
#define SAY_SLAY_2                               -1999658
#define SAY_DEATH                                -1999657

struct MANGOS_DLL_DECL boss_palehoofAI : public ScriptedAI
{
    boss_palehoofAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

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

        switch(rand()%2)
        {
            case 0:
                DoScriptText(SAY_SLAY_1, m_creature);
                break;
            case 1:
                DoScriptText(SAY_SLAY_2, m_creature);
                break;
        }
    }
};

CreatureAI* GetAI_boss_palehoof(Creature* pCreature)
{
    return new boss_palehoofAI(pCreature);
}

void AddSC_boss_palehoof()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_palehoof";
    newscript->GetAI = GetAI_boss_palehoof;
    newscript->RegisterSelf();
}

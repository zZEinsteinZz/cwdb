/* Script Data Start
SDName: Boss svala
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = 'boss_svala' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_CALL_FLAMES                        48258
#define SPELL_RITUAL_OF_THE_SWORD                48276 //Effect #1 Teleport,  Effect #2 Dummy
#define SPELL_SINSTER_STRIKE                     15667

//Yells
#define SAY_DIALOG_WITH_ARTHAS_1              -1999635
#define SAY_DIALOG_WITH_ARTHAS_2              -1999634
#define SAY_DIALOG_WITH_ARTHAS_3              -1999633
#define SAY_AGGRO                             -1999645
#define SAY_SLAY_1                            -1999644
#define SAY_SLAY_2                            -1999643
#define SAY_SLAY_3                            -1999642
#define SAY_DEATH                             -1999641
#define SAY_SACRIFICE_PLAYER_1                -1999640
#define SAY_SACRIFICE_PLAYER_2                -1999639
#define SAY_SACRIFICE_PLAYER_3                -1999638
#define SAY_SACRIFICE_PLAYER_4                -1999637
#define SAY_SACRIFICE_PLAYER_5                -1999636

struct MANGOS_DLL_DECL boss_svalaAI : public ScriptedAI
{
    boss_svalaAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

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
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature);break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature);break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature);break;
        }
    }
};

CreatureAI* GetAI_boss_svala(Creature* pCreature)
{
    return new boss_svalaAI(pCreature);
}

void AddSC_boss_svala()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_svala";
    newscript->GetAI = GetAI_boss_svala;
    newscript->RegisterSelf();
}

/* Script Data Start
SDName: Boss varos
SDAuthor: LordVanMartin
SD%Complete: 
SDComment: 
SDCategory: 
Script Data End */

/*** SQL START *** 
update creature_template set scriptname = '' where entry = '';
*** SQL END ***/
#include "precompiled.h"

//Spells
#define SPELL_ENERGIZE_CORES                      50785 //Damage 5938 to 6562, effec2 Triggers 54069, effect3 Triggers 56251
#define SPELL_ENERGIZE_CORES_TRIGGER_1            54069 
#define SPELL_ENERGIZE_CORES_TRIGGER_2            56251 
#define SPELL_ENERGIZE_CORES_2                    59372 //Damage 9025 to 9975, effect2 Triggers 54069, effect 56251
#define SPELL_CALL_AZURE_RING_CAPTAIN             51002 //Effect    Send Event (12229)
#define SPELL_CALL_AZURE_RING_CAPTAIN_2           51006 //Effect    Send Event (10665)
#define SPELL_CALL_AZURE_RING_CAPTAIN_3           51007 //Effect    Send Event (18454)
#define SPELL_CALL_AZURE_RING_CAPTAIN_4           51008 //Effect    Send Event (18455)
#define SPELL_CALL_AMPLIFY_MAGIC                  51054
#define SPELL_CALL_AMPLIFY_MAGIC_2                59371

//Yell
#define SAY_AGGRO                              -1999747 
#define SAY_KILL_1                             -1999746
#define SAY_KILL_2                             -1999745  
#define SAY_DEATH                              -1999744 
#define SAY_STRIKE_1                           -1999743    
#define SAY_STRIKE_2                           -1999742    
#define SAY_STRIKE_3                           -1999741     
#define SAY_SPAWN                              -1999740 


struct MANGOS_DLL_DECL boss_varosAI : public ScriptedAI
{
    boss_varosAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

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
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_KILL_1, m_creature);break;
            case 1: DoScriptText(SAY_KILL_2, m_creature);break;
        }
    }
};

CreatureAI* GetAI_boss_varos(Creature* pCreature)
{
    return new boss_varosAI(pCreature);
}

void AddSC_boss_varos()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="boss_varos";
    newscript->GetAI = GetAI_boss_varos;
    newscript->RegisterSelf();
}

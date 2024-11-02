// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Components/CombatComponent.h"

UBTT_Attack::UBTT_Attack(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
	// Get the combat component
	UCombatComponent* pCombatComp = Cast<UCombatComponent>(a_pTreeComp.GetAIOwner()->GetPawn()->GetComponentByClass(UCombatComponent::StaticClass()));

	// Return early if this controller doesn't have a combat component
	if (!pCombatComp)
		return EBTNodeResult::Failed;

	// Check if any previous attacks are finished
	if (!pCombatComp->IsAttacking())
	{
		// Get blackboard from ai controller
		UBlackboardComponent* pBlackboard = Cast<UBlackboardComponent>(a_pTreeComp.GetAIOwner()->GetComponentByClass(UBlackboardComponent::StaticClass()));

		// Get bool value from key to check whether we can attack
		bool bCanKickAttack = pBlackboard->GetValueAsBool(GetSelectedBlackboardKey());
		if (bCanKickAttack)
		{
			// Run Boss KickAttack method
			pCombatComp->Attack(Attack);
		}
	}

	// Finish execution
	FinishLatentTask(a_pTreeComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

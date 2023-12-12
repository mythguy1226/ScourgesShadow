// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SlashAttack.h"
#include "Boss_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BossKeys.h"

UBTT_SlashAttack::UBTT_SlashAttack(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Slash Attack");
}

EBTNodeResult::Type UBTT_SlashAttack::ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
	// Get AI Controller and Boss from pawn
	ABoss_Controller* pAIController = Cast<ABoss_Controller>(a_pTreeComp.GetAIOwner());
	ABoss* pBoss = Cast<ABoss>(pAIController->GetPawn());

	// Check if SlashAttack montage is finished
	if (!pBoss->GetCombatComponent()->IsAttacking())
	{
		// Get bool value from key to check whether we can SlashAttack
		bool bCanSlashAttack = pAIController->GetBlackboard()->GetValueAsBool(BossKeys::isPlayerInRange);
		if (bCanSlashAttack)
		{
			// Run Boss SlashAttack method
			pBoss->SlashAttack();
		}
	}

	// Finish execution
	FinishLatentTask(a_pTreeComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

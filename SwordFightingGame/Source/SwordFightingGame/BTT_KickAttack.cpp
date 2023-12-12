// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_KickAttack.h"
#include "Boss_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BossKeys.h"

UBTT_KickAttack::UBTT_KickAttack(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Kick Attack");
}

EBTNodeResult::Type UBTT_KickAttack::ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
	// Get AI Controller and Boss from pawn
	ABoss_Controller* pAIController = Cast<ABoss_Controller>(a_pTreeComp.GetAIOwner());
	ABoss* pBoss = Cast<ABoss>(pAIController->GetPawn());

	// Check if KickAttack montage is finished
	if (!pBoss->GetCombatComponent()->IsAttacking())
	{
		// Get bool value from key to check whether we can KickAttack
		bool bCanKickAttack = pAIController->GetBlackboard()->GetValueAsBool(BossKeys::isPlayerInRange);
		if (bCanKickAttack)
		{
			// Run Boss KickAttack method
			pBoss->KickAttack();
		}
	}

	// Finish execution
	FinishLatentTask(a_pTreeComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

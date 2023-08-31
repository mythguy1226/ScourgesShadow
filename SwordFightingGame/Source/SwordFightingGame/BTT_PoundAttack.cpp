// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_PoundAttack.h"
#include "Boss_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BossKeys.h"

UBTT_PoundAttack::UBTT_PoundAttack(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Pound Attack");
}

EBTNodeResult::Type UBTT_PoundAttack::ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
	// Get AI Controller and Boss from pawn
	ABoss_Controller* pAIController = Cast<ABoss_Controller>(a_pTreeComp.GetAIOwner());
	ABoss* pBoss = Cast<ABoss>(pAIController->GetPawn());

	// Check if KickAttack montage is finished
	if (AttackMontageFinished(pBoss))
	{
		// Get bool value from key to check whether we can KickAttack
		bool bCanKickAttack = pAIController->GetBlackboard()->GetValueAsBool(BossKeys::isPlayerInRange);
		if (bCanKickAttack)
		{
			// Run Boss KickAttack method
			pBoss->PoundAttack();
		}
	}

	// Finish execution
	FinishLatentTask(a_pTreeComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

bool UBTT_PoundAttack::AttackMontageFinished(ABoss* a_pBoss)
{
	// Get the anim instance
	UAnimInstance* pAnimInstance = a_pBoss->GetMesh()->GetAnimInstance();
	if (pAnimInstance != nullptr)
	{
		return pAnimInstance->Montage_GetIsStopped(a_pBoss->m_pKickAttackMontage) && pAnimInstance->Montage_GetIsStopped(a_pBoss->m_pSlashAttackMontage) && pAnimInstance->Montage_GetIsStopped(a_pBoss->m_pPoundAttackMontage);
	}
	return false;
}

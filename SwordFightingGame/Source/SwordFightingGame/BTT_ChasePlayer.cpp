// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Boss_Controller.h"
#include "Boss.h"
#include "BossKeys.h"
#include "Kismet/GameplayStatics.h"

UBTT_ChasePlayer::UBTT_ChasePlayer(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
	// Get AI Controller
	auto const pAIController = Cast<ABoss_Controller>(a_pTreeComp.GetAIOwner());

	// Get Nav System
	UNavigationSystemV1* pNavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (pNavSystem)
	{
		// Get the boss ref
		ABoss* pBoss = Cast<ABoss>(pAIController->GetPawn());
		
		// Only chase if alive
		if (pBoss->m_fHealth > 0)
		{
			// Get location of the player
			FVector pLocation = pAIController->GetBlackboard()->GetValueAsVector(BossKeys::targetLocation);

			// Tell the AI to move towards player
			pAIController->MoveToLocation(pLocation);
		}
	}

	// Finish execution
	FinishLatentTask(a_pTreeComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AI/Boss_Controller.h"
#include "AI/Boss.h"
#include "Data/BossKeys.h"
#include "Kismet/GameplayStatics.h"

UBTT_ChasePlayer::UBTT_ChasePlayer(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
	// Get Nav System
	UNavigationSystemV1* pNavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (pNavSystem)
	{
		// Get blackboard from ai controller
		UBlackboardComponent* pBlackboard = Cast<UBlackboardComponent>(a_pTreeComp.GetAIOwner()->GetComponentByClass(UBlackboardComponent::StaticClass()));

		// Get location of the player
		FVector pLocation = pBlackboard->GetValueAsVector(GetSelectedBlackboardKey());

		// Tell the AI to move towards player
		a_pTreeComp.GetAIOwner()->MoveToLocation(pLocation);
	}

	// Finish execution
	FinishLatentTask(a_pTreeComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

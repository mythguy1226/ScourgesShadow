// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AI/Boss_Controller.h"
#include "Data/BossKeys.h"
#include "Kismet/GameplayStatics.h"

UBTT_FindPlayerLocation::UBTT_FindPlayerLocation(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTT_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
	// Get Nav System
	UNavigationSystemV1* pNavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	// Get player reference
	APlayerController* pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APawn* pPlayer = pPlayerController->GetPawn();

	// Get blackboard from ai controller
	UBlackboardComponent* pBlackboard = Cast<UBlackboardComponent>(a_pTreeComp.GetAIOwner()->GetComponentByClass(UBlackboardComponent::StaticClass()));

	if (pNavSystem)
	{
		// Setting target location to player location
		pBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), pPlayer->GetActorLocation());
	}

	// Finish execution
	FinishLatentTask(a_pTreeComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

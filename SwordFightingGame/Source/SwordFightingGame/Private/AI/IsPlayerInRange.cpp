// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/IsPlayerInRange.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Boss_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Data/BossKeys.h"

UIsPlayerInRange::UIsPlayerInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Range");
}

void UIsPlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
	// Super Call 
	Super::OnBecomeRelevant(a_pTreeComp, a_pNodeMem);

	// Get blackboard from ai controller
	UBlackboardComponent* pBlackboard = Cast<UBlackboardComponent>(a_pTreeComp.GetAIOwner()->GetComponentByClass(UBlackboardComponent::StaticClass()));

	// Set blackboard key to whether or not player is in range
	pBlackboard->SetValueAsBool(GetSelectedBlackboardKey(), a_pTreeComp.GetAIOwner()->GetPawn()->GetDistanceTo(Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) <= m_fRange);
}

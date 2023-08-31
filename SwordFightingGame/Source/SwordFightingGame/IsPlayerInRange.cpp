// Fill out your copyright notice in the Description page of Project Settings.


#include "IsPlayerInRange.h"
#include "Kismet/GameplayStatics.h"
#include "Boss_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BossKeys.h"

UIsPlayerInRange::UIsPlayerInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Range");
}

void UIsPlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
	// Super Call 
	Super::OnBecomeRelevant(a_pTreeComp, a_pNodeMem);

	// Get AI Controller and pawn
	ABoss_Controller* pAIController = Cast<ABoss_Controller>(a_pTreeComp.GetAIOwner());
	APawn* pBoss = pAIController->GetPawn();

	// Get Player Controller and pawn
	APlayerController* pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Set blackboard key to whether or not player is in range
	pAIController->GetBlackboard()->SetValueAsBool(BossKeys::isPlayerInRange, pBoss->GetDistanceTo(pPlayerController->GetPawn()) <= m_fRange);
}

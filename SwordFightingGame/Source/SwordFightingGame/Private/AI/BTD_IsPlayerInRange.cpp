// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTD_IsPlayerInRange.h"
#include "Data/BossKeys.h"
#include "AI/Boss_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_IsPlayerInRange::UBTD_IsPlayerInRange()
{
	NodeName = TEXT("Is Player In Range");
}

bool UBTD_IsPlayerInRange::CalculateRawConditionValue(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem) const
{
	// Super call to decorator
	bool bSuccess = Super::CalculateRawConditionValue(a_pTreeComp, a_pNodeMem);
	if (!bSuccess) return false;

	// Get blackboard from ai controller
	UBlackboardComponent* pBlackboard = Cast<UBlackboardComponent>(a_pTreeComp.GetAIOwner()->GetComponentByClass(UBlackboardComponent::StaticClass()));

	// Get the player in range value from key
	bool inRange = pBlackboard->GetValueAsBool(BossKeys::isPlayerInRange);

	// Return the key value
	return inRange;
}

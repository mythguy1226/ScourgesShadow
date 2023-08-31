// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsPlayerInRange.h"
#include "BossKeys.h"
#include "Boss_Controller.h"
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

	// Get the controller
	ABoss_Controller* pController = Cast<ABoss_Controller>(a_pTreeComp.GetAIOwner());

	// Get the player in range value from key
	bool inRange = pController->GetBlackboard()->GetValueAsBool(BossKeys::isPlayerInRange);

	// Return the key value
	return inRange;
}

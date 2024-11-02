// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_IsPlayerInRange.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API UBTD_IsPlayerInRange : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	// Constructor
	UBTD_IsPlayerInRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem) const override;
	
};

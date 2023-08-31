// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "RandomChance.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API URandomChance : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	// Constructor
	URandomChance();

	// Field for chance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float m_fChance = 100.0f;
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem) const override;
};

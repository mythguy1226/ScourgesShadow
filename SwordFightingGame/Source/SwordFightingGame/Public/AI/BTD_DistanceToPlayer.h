// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_DistanceToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API UBTD_DistanceToPlayer : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	// Constructor
	UBTD_DistanceToPlayer();

	// Field for distances
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float m_fFarDistance = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float m_fNearDistance = 100.0f;
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem) const override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_TurnToFaceTarget.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API UBTT_TurnToFaceTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_TurnToFaceTarget(FObjectInitializer const& a_pObjectInit);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LookRotationRate = 0.25f;
};

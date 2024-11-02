// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API UBTT_ChangeSpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fNewSpeed;

	UBTT_ChangeSpeed(FObjectInitializer const& a_pObjectInit);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem);
};

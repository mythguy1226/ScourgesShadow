// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Boss.h"
#include "BTT_SlashAttack.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API UBTT_SlashAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_SlashAttack(FObjectInitializer const& a_pObjectInit);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem);
	bool AttackMontageFinished(ABoss* a_pBoss);
};

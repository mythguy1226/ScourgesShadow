// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Boss.h"
#include "BTT_KickAttack.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API UBTT_KickAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_KickAttack(FObjectInitializer const& a_pObjectInit);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem);
	bool AttackMontageFinished(ABoss* a_pBoss);
};

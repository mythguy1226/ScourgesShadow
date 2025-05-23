// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StatsComponent.h"
#include "BossStatsComponent.generated.h"

class ABoss;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDFIGHTINGGAME_API UBossStatsComponent : public UStatsComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

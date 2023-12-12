// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDFIGHTINGGAME_API UBossStatsComponent : public UActorComponent
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

	// Tracker for boss stun meter
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fMaxStunResistance = 100.0f;
	float m_fStunMeter = m_fMaxStunResistance;
	
	// Methods for managing stun bar
	void DecrementStun(float a_fStunAmount);
	void ResetStunMeter();
	bool IsStunMeterEmpty();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDFIGHTINGGAME_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsComponent();

	// Reference to owning character
	ACharacter* m_pOwningCharacter = nullptr;

	// *** Max Stamina ***
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fMaxStamina = 100.0f;

	// Set default to max
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float m_fStamina = m_fMaxStamina;

	// Rate at which stamina regenerates
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float m_fStaminaRegenerationRate = 0.07f;

	// Tracker for stun meter
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fMaxStunResistance = 100.0f;
	float m_fStunMeter = m_fMaxStunResistance;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Check if player has enough stamina to perform
	bool DoesMeetStaminaRequirement(float a_fStaminaReq);

	// Method used for exhausting stamina
	void UseStamina(float a_fStaminaUsed);

	// Methods for managing stun bar
	void DecrementStun(float a_fStunAmount);
	void ResetStunMeter();
	bool IsStunMeterEmpty();
};

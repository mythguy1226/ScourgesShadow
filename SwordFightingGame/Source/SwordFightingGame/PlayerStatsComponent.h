// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDFIGHTINGGAME_API UPlayerStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatsComponent();

	// *** Max Stamina ***
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fMaxStamina = 100.0f;

	// Set default to max
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float m_fStamina = m_fMaxStamina;

	// Rate at which stamina regenerates
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float m_fStaminaRegenerationRate = 0.07f;

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
};

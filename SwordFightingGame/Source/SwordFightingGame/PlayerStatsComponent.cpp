// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatsComponent.h"

// Sets default values for this component's properties
UPlayerStatsComponent::UPlayerStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Slowly Regenerate Stamina
	m_fStamina += m_fStaminaRegenerationRate;

	// Cap the stamina at the max
	if (m_fStamina > m_fMaxStamina)
		m_fStamina = m_fMaxStamina;
}

// Return whether or not player meets stamina requirements
bool UPlayerStatsComponent::DoesMeetStaminaRequirement(float a_fStaminaReq)
{
	return m_fStamina >= a_fStaminaReq;
}

// Set stamina to be a subtraction from amount used
void UPlayerStatsComponent::UseStamina(float a_fStaminaUsed)
{
	m_fStamina -= a_fStaminaUsed;
}


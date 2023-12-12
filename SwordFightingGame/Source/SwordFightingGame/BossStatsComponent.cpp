// Fill out your copyright notice in the Description page of Project Settings.


#include "BossStatsComponent.h"

// Sets default values for this component's properties
UBossStatsComponent::UBossStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// Fill up the stun meter
	m_fStunMeter = m_fMaxStunResistance;
	
}


// Called every frame
void UBossStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Method for decrementing stun meter
void UBossStatsComponent::DecrementStun(float a_fStunAmount)
{
	m_fStunMeter -= a_fStunAmount;
}

// Method for resetting stun meter back to max
void UBossStatsComponent::ResetStunMeter()
{
	m_fStunMeter = m_fMaxStunResistance;
}

// Method for checking if stun meter is empty
bool UBossStatsComponent::IsStunMeterEmpty()
{
	return m_fStunMeter <= 0.0f;
}


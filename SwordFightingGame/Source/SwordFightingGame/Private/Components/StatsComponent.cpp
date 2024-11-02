// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatsComponent.h"
#include "Components/CombatComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set reference of character to owning actor
	m_pOwningCharacter = Cast<ACharacter>(GetOwner());

	// Fill up stamina
	m_fStamina = m_fMaxStamina;

	// Fill up the stun meter
	m_fStunMeter = m_fMaxStunResistance;
}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Slowly Regenerate Stamina
	m_fStamina += m_fStaminaRegenerationRate;

	// Cap the stamina at the max
	if (m_fStamina > m_fMaxStamina)
		m_fStamina = m_fMaxStamina;
}

// Return whether or not player meets stamina requirements
bool UStatsComponent::DoesMeetStaminaRequirement(float a_fStaminaReq)
{
	return m_fStamina >= a_fStaminaReq;
}

// Set stamina to be a subtraction from amount used
void UStatsComponent::UseStamina(float a_fStaminaUsed)
{
	m_fStamina -= a_fStaminaUsed;
}

// Method for decrementing stun meter
void UStatsComponent::DecrementStun(float a_fStunAmount)
{
	// Decrease by stun amount
	m_fStunMeter -= a_fStunAmount;

	// Stun the boss if meter is empty
	if (IsStunMeterEmpty())
	{
		// Get the combat component
		UCombatComponent* pCombatComp = Cast<UCombatComponent>(m_pOwningCharacter->GetComponentByClass(UCombatComponent::StaticClass()));

		// Early return if combat component is null
		if (!pCombatComp)
			return;

		// Null check the stun montage
		if (pCombatComp->m_pHurtMontage)
			m_pOwningCharacter->GetMesh()->GetAnimInstance()->Montage_Play(pCombatComp->m_pHurtMontage);

		// Refill the stun meter back to its max
		ResetStunMeter();
	}
}

// Method for resetting stun meter back to max
void UStatsComponent::ResetStunMeter()
{
	m_fStunMeter = m_fMaxStunResistance;
}

// Method for checking if stun meter is empty
bool UStatsComponent::IsStunMeterEmpty()
{
	return m_fStunMeter <= 0.0f;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "EvasionComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UEvasionComponent::UEvasionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEvasionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set reference of owning character
	m_pOwningCharacter = Cast<ACharacter>(GetOwner());
	
}


// Called every frame
void UEvasionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEvasionComponent::Dodge(bool a_bSprinting, bool a_bTargetLocked)
{
	// Get owning character's anim instance
	UAnimInstance* pAnimInst = m_pOwningCharacter->GetMesh()->GetAnimInstance();

	// Can't dodge if already dodging
	if (!m_bIsDodging)
	{
		// Enable dodging boolean
		m_bIsDodging = true;

		// Check for target lock
		if (!a_bTargetLocked)
		{
			// When sprinting, fall into the dodge roll
			if (a_bSprinting)
			{
				pAnimInst->Montage_Play(m_pFallDodgeMontage);
			}
			else
			{
				pAnimInst->Montage_Play(m_pDodgeMontage);
			}
		}
		else
		{
			// Get the value from player rotation
			float fRotValue = pAnimInst->CalculateDirection(m_pOwningCharacter->GetVelocity(), m_pOwningCharacter->GetActorRotation());

			// Forward Rolling
			if ((fRotValue >= 0 && fRotValue <= 25) || (fRotValue <= 0 && fRotValue >= -25))
			{
				pAnimInst->Montage_Play(m_pLockDodgeForwardMontage);
				m_pOwningCharacter->LaunchCharacter(m_pOwningCharacter->GetActorForwardVector() * 1000, true, true);
			}
			else if ((fRotValue > 25 && fRotValue < 80)) // Forward Right Rolling
			{
				pAnimInst->Montage_Play(m_pLockDodgeForwardRMontage);
				m_pOwningCharacter->LaunchCharacter((m_pOwningCharacter->GetActorForwardVector() + m_pOwningCharacter->GetActorRightVector()) * 500, true, true);
			}
			else if ((fRotValue < -25 && fRotValue > -80)) // Forward Left Rolling
			{
				pAnimInst->Montage_Play(m_pLockDodgeForwardLMontage);
				m_pOwningCharacter->LaunchCharacter((m_pOwningCharacter->GetActorForwardVector() + (m_pOwningCharacter->GetActorRightVector() * -1)) * 500, true, true);
			}
			else if ((fRotValue >= 80 && fRotValue <= 100)) // Right Rolling
			{
				pAnimInst->Montage_Play(m_pLockDodgeRightMontage);
				m_pOwningCharacter->LaunchCharacter(m_pOwningCharacter->GetActorRightVector() * 1000, true, true);
			}
			else if ((fRotValue <= -80 && fRotValue >= -100)) // Left Rolling
			{
				pAnimInst->Montage_Play(m_pLockDodgeLeftMontage);
				m_pOwningCharacter->LaunchCharacter(m_pOwningCharacter->GetActorRightVector() * -1000, true, true);
			}
			else if ((fRotValue <= 180 && fRotValue >= 155) || (fRotValue >= -180 && fRotValue <= -155)) // Backward Rolling
			{
				pAnimInst->Montage_Play(m_pLockDodgeBackMontage);
				m_pOwningCharacter->LaunchCharacter(m_pOwningCharacter->GetActorForwardVector() * -1000, true, true);
			}
			else if ((fRotValue > -155 && fRotValue < -100)) // Backward Left Rolling
			{
				pAnimInst->Montage_Play(m_pLockDodgeBackLMontage);
				m_pOwningCharacter->LaunchCharacter((m_pOwningCharacter->GetActorForwardVector() + m_pOwningCharacter->GetActorRightVector()) * -500, true, true);
			}
			else if ((fRotValue < 155 && fRotValue > 100)) // Backward Right Rolling
			{
				pAnimInst->Montage_Play(m_pLockDodgeBackRMontage);
				m_pOwningCharacter->LaunchCharacter(((m_pOwningCharacter->GetActorForwardVector() * -1) + m_pOwningCharacter->GetActorRightVector()) * 500, true, true);
			}
		}
	}
}


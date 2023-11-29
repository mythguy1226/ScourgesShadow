// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Boss.h"
#include "SwordFightingGameCharacter.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::GenerateHitSphere(FVector a_vLocation, float a_fRadius, float a_fDamage, bool a_bDebug, bool a_bKnockback)
{
	// Init hit results
	TArray<FHitResult> outHits;
	TArray<AActor*> ignoredActors;
	ignoredActors.Add(GetOwner());
	
	// Use Sphere Shape
	FCollisionShape sphereShape;
	sphereShape.ShapeType = ECollisionShape::Sphere;
	sphereShape.SetSphere(a_fRadius);
	
	// Set the query params
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(GetOwner());
	
	// Sweep Trace at typical contact point of swing
	GetWorld()->SweepMultiByChannel(outHits, a_vLocation, a_vLocation, FQuat::Identity, ECollisionChannel::ECC_Pawn, sphereShape, queryParams);
	
	// Debug Trace to show visual representation
	if (a_bDebug)
	{
		// Sweep Trace at typical contact point of swing with debugging visuals
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), a_vLocation, a_vLocation,
			a_fRadius,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn), true, ignoredActors,
			EDrawDebugTrace::ForDuration, outHits, true, FColor::Red);
	}
	
	// Check that a collision happened
	if (outHits.Num() > 0)
	{
		// Iterate through the hit results
		for (auto i = outHits.CreateIterator(); i; i++)
		{
			// Try to cast to a boss
			ABoss* pBoss = Cast<ABoss>(i->GetActor());
			if (pBoss) // Continue if valid
			{
				HandleBossDamage(pBoss, a_vLocation, a_fDamage);
				break;
			}
			
			// Try to cast to the player
			ASwordFightingGameCharacter* pPlayer = Cast<ASwordFightingGameCharacter>(i->GetActor());
			if (pPlayer) // Continue if valid
			{
				HandlePlayerDamage(pPlayer, i->Location, a_fDamage, a_bKnockback);
				break;
			}
		}
	}
}

void UCombatComponent::GenerateHitCapsule(FVector a_vBeginLoc, FVector a_vEndLoc, float a_fRadius, float a_fDamage, bool a_bDebug, bool a_bKnockback)
{
	// Init hit results
	TArray<FHitResult> outHits;
	TArray<AActor*> ignoredActors;
	ignoredActors.Add(GetOwner());
	
	// Use Sphere Shape
	FCollisionShape sphereShape;
	sphereShape.ShapeType = ECollisionShape::Sphere;
	sphereShape.SetSphere(a_fRadius);
	
	// Set the query params
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(GetOwner());
	
	// Sweep Trace at typical contact point of swing
	GetWorld()->SweepMultiByChannel(outHits, a_vBeginLoc, a_vEndLoc, FQuat::Identity, ECollisionChannel::ECC_Pawn, sphereShape, queryParams);
	
	// Debug Trace to show visual representation
	if (a_bDebug)
	{
		// Sweep Trace at typical contact point of swing with debugging visuals
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), a_vBeginLoc, a_vEndLoc,
			a_fRadius,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn), true, ignoredActors,
			EDrawDebugTrace::ForDuration, outHits, true, FColor::Red);
	}
	
	// Check that a collision happened
	if (outHits.Num() > 0)
	{
		// Iterate through the hit results
		for (auto i = outHits.CreateIterator(); i; i++)
		{
			// Try to cast to a boss
			ABoss* pBoss = Cast<ABoss>(i->GetActor());
			if (pBoss) // Continue if valid
			{
				HandleBossDamage(pBoss, a_vEndLoc, a_fDamage);
				break;
			}
			
			// Try to cast to the player
			ASwordFightingGameCharacter* pPlayer = Cast<ASwordFightingGameCharacter>(i->GetActor());
			if (pPlayer) // Continue if valid
			{
				HandlePlayerDamage(pPlayer, i->Location, a_fDamage, a_bKnockback);
				break;
			}
		}
	}
}

void UCombatComponent::HandleBossDamage(ABoss* a_pBoss, FVector a_vLoc, float a_fDamage)
{
	// Deal damage
	a_pBoss->TakeDamage(a_fDamage);

	// Spawn Blood Particle at the hit location
	if (m_pImpactParticle != nullptr)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pImpactParticle, a_vLoc, FQuat::Identity.Rotator());

	// Play damage indicator sound
	if (m_pImpactSound != nullptr)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pImpactSound, GetOwner()->GetActorLocation());

	// Handle stun meter
	if (a_pBoss->m_pHurtMontage != nullptr)
	{
		// Deplete stun meter
		a_pBoss->m_fStunMeter -= 15.0f;
		if (a_pBoss->m_fStunMeter <= 0.0f) // Stun the boss
		{
			a_pBoss->GetMesh()->GetAnimInstance()->Montage_Play(a_pBoss->m_pHurtMontage);
			a_pBoss->m_fStunMeter = 100.0f;
		}
	}
}

void UCombatComponent::HandlePlayerDamage(ASwordFightingGameCharacter* a_pPlayer, FVector a_vLoc, float a_fDamage, bool a_bKnockback)
{
	// Return if player is dying or dead
	if (a_pPlayer->m_fHealth <= 0)
	{
		return;
	}

	// Don't deal damage if player is dodging
	if (!a_pPlayer->m_bIsDodging && !a_pPlayer->IsStaggered() && !a_pPlayer->IsDying())
	{
		// Play hurt animation and deal damage
		if (a_pPlayer->m_pHurtMontage != nullptr && !a_bKnockback)
		{
			// If player isn't block do normal damage
			if (!a_pPlayer->m_bIsBlocking)
			{
				// Play hurt sound
				if (a_pPlayer->m_pHurtSound != nullptr)
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), a_pPlayer->m_pHurtSound, GetOwner()->GetActorLocation());

				a_pPlayer->GetMesh()->GetAnimInstance()->Montage_Play(a_pPlayer->m_pHurtMontage);
				a_pPlayer->TakeDamage(a_fDamage);
			}
			else // Otherwise negate some damage
			{
				// Play impact sound
				if (a_pPlayer->m_pBlockSound != nullptr)
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), a_pPlayer->m_pBlockSound, GetOwner()->GetActorLocation());

				a_pPlayer->GetMesh()->GetAnimInstance()->Montage_Play(a_pPlayer->m_pShieldImpactMontage);
				a_pPlayer->TakeDamage(a_fDamage / 2.0f);
			}
		}

		// Handle Forceful attacks
		if (a_bKnockback)
		{
			// Spawn Blood Particle at the hit location
			if (a_pPlayer->m_pBloodParticle != nullptr)
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), a_pPlayer->m_pBloodParticle, a_vLoc, FQuat::Identity.Rotator());

			// Play damage indicator sound
			if (a_pPlayer->m_pKnockbackSound != nullptr)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), a_pPlayer->m_pKnockbackSound, a_pPlayer->GetActorLocation());

			if (!a_pPlayer->GetMesh()->GetAnimInstance()->Montage_IsPlaying(a_pPlayer->m_pKnockbackMontage))
			{
				FVector playerToEnemy = a_pPlayer->GetActorLocation() - GetOwner()->GetActorLocation();
				a_pPlayer->LaunchCharacter(playerToEnemy * 30, true, true);
				a_pPlayer->GetMesh()->GetAnimInstance()->Montage_Play(a_pPlayer->m_pKnockbackMontage);
				a_pPlayer->TakeDamage(a_fDamage);
			}
		}
	}
}


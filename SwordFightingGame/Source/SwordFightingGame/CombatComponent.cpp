// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

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

void UCombatComponent::GenerateHitSphere(FVector a_vLocation, float a_fRadius, float a_fDamage, bool a_bDebug)
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
			// Try to cast to the player
			//UCombatComponent* pVictim = Cast<UCombatComponent>(i->GetActor());
			//if (pBoss) // Continue if valid
			//{
			//	// Deal damage
			//	pBoss->TakeDamage(a_fDamage);
			//
			//	// Spawn Blood Particle at the hit location
			//	if (m_pBloodParticle != nullptr)
			//		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pBloodParticle, a_vLocation, FQuat::Identity.Rotator());
			//
			//	// Play damage indicator sound
			//	if (m_pSlashImpactSound != nullptr)
			//		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pSlashImpactSound, GetOwner()->GetActorLocation());
			//
			//	// Handle stun meter
			//	if (pBoss->m_pHurtMontage != nullptr)
			//	{
			//		// Deplete stun meter
			//		pBoss->m_fStunMeter -= 15.0f;
			//		if (pBoss->m_fStunMeter <= 0.0f) // Stun the boss
			//		{
			//			pBoss->GetMesh()->GetAnimInstance()->Montage_Play(pBoss->m_pHurtMontage);
			//			pBoss->m_fStunMeter = 100.0f;
			//		}
			//	}
			//	break;
			//}
		}
	}
}

void UCombatComponent::GenerateHitCapsule(FVector a_vBeginLoc, FVector a_vEndLoc, float a_fRadius, float a_fDamage, bool a_bDebug)
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
			// Try to cast to the player
			//UCombatComponent* pVictim = Cast<UCombatComponent>(i->GetActor());
			//if (pBoss) // Continue if valid
			//{
			//	// Deal damage
			//	pBoss->TakeDamage(a_fDamage);
			//
			//	// Spawn Blood Particle at the hit location
			//	if (m_pBloodParticle != nullptr)
			//		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pBloodParticle, a_vLocation, FQuat::Identity.Rotator());
			//
			//	// Play damage indicator sound
			//	if (m_pSlashImpactSound != nullptr)
			//		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pSlashImpactSound, GetOwner()->GetActorLocation());
			//
			//	// Handle stun meter
			//	if (pBoss->m_pHurtMontage != nullptr)
			//	{
			//		// Deplete stun meter
			//		pBoss->m_fStunMeter -= 15.0f;
			//		if (pBoss->m_fStunMeter <= 0.0f) // Stun the boss
			//		{
			//			pBoss->GetMesh()->GetAnimInstance()->Montage_Play(pBoss->m_pHurtMontage);
			//			pBoss->m_fStunMeter = 100.0f;
			//		}
			//	}
			//	break;
			//}
		}
	}
}


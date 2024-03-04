// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetLockComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "Boss.h"

// Sets default values for this component's properties
UTargetLockComponent::UTargetLockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTargetLockComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTargetLockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTargetLockComponent::ToggleTargetLock()
{
	// Turn off if already on
	if (m_bIsTargetLocked)
	{
		DisableLock();
		return;
	}

	// Generate a lock query to find an actor to lock onto
	GenerateLockQuery(GetOwner()->GetActorLocation(), GetOwner()->GetActorForwardVector(), m_fLockQueryRange, m_fLockQueryRadius, m_bDebugMode);
}

void UTargetLockComponent::GenerateLockQuery(FVector a_vBeginLoc, FVector a_vForward, float a_fRange, float a_fRadius, bool a_bDebug)
{
	// Calculate the end point
	FVector vEndLoc = a_vBeginLoc + (a_vForward * a_fRange);

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
	GetWorld()->SweepMultiByChannel(outHits, a_vBeginLoc, vEndLoc, FQuat::Identity, ECollisionChannel::ECC_Pawn, sphereShape, queryParams);

	// Debug Trace to show visual representation
	if (a_bDebug)
	{
		// Sweep Trace at typical contact point of swing with debugging visuals
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), a_vBeginLoc, vEndLoc,
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
			// Set locked actor to the hit actor if they can be cast to a boss or enemy
			if (Cast<AEnemy>(i->GetActor()) || Cast<ABoss>(i->GetActor()))
			{
				// Avoid repeated locking
				if (i->GetActor() == m_pTargetLockedActor)
					continue;
				
				m_pTargetLockedActor = i->GetActor();

				// Toggle the target lock
				m_bIsTargetLocked = true;

				break;
			}
		}
	}
}

void UTargetLockComponent::GetNextTarget(float a_fDirection)
{
	// Return early if locked actor is null
	if (!m_pTargetLockedActor)
		return;

	// Get the adjacent targets in the direction the player chooses
	// via a new lock query
	GenerateLockQuery(m_pTargetLockedActor->GetActorLocation(), GetOwner()->GetActorRightVector() * a_fDirection, m_fLockQueryRange, m_fLockQueryRadius, m_bDebugMode);
}

void UTargetLockComponent::DisableLock()
{
	// Disable locked flag and set actor pointer to be null
	m_bIsTargetLocked = false;
	m_pTargetLockedActor = nullptr;
}


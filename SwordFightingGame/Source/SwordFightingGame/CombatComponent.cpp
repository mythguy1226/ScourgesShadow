// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Boss.h"
#include "SwordFightingGameCharacter.h"
#include "EvasionComponent.h"

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

	// Set health to its max
	m_fHealth = m_fMaxHealth;
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::GenerateHitSphere(FVector a_vLocation, float a_fRadius, FAttackStats a_sAttackStats, bool a_bDebug)
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
			// Check if actor is part of damaged actor set
			if (m_sDamagedActors.Contains(i->GetActor()))
				continue;
			else // Add to set if not
				m_sDamagedActors.Add(i->GetActor());

			// Try to cast to a boss
			ABoss* pBoss = Cast<ABoss>(i->GetActor());
			if (pBoss) // Continue if valid
			{
				HandleBossDamage(pBoss, a_vLocation, a_sAttackStats.sDamage);
				break;
			}
			
			// Try to cast to the player
			ASwordFightingGameCharacter* pPlayer = Cast<ASwordFightingGameCharacter>(i->GetActor());
			if (pPlayer) // Continue if valid
			{
				HandlePlayerDamage(pPlayer, i->Location, a_sAttackStats.sDamage, a_sAttackStats.sKnockback);
				break;
			}
		}
	}
}

void UCombatComponent::GenerateHitCapsule(FVector a_vBeginLoc, FVector a_vEndLoc, float a_fRadius, FAttackStats a_sAttackStats, bool a_bDebug)
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
			// Check if actor is part of damaged actor set
			if (m_sDamagedActors.Contains(i->GetActor()))
				continue;
			else // Add to set if not
				m_sDamagedActors.Add(i->GetActor());


			// Try to cast to a boss
			ABoss* pBoss = Cast<ABoss>(i->GetActor());
			if (pBoss) // Continue if valid
			{
				HandleBossDamage(pBoss, a_vEndLoc, a_sAttackStats.sDamage);
				break;
			}
			
			// Try to cast to the player
			ASwordFightingGameCharacter* pPlayer = Cast<ASwordFightingGameCharacter>(i->GetActor());
			if (pPlayer) // Continue if valid
			{
				HandlePlayerDamage(pPlayer, i->Location, a_sAttackStats.sDamage, a_sAttackStats.sKnockback);
				break;
			}
		}
	}
}

void UCombatComponent::HandleBossDamage(ABoss* a_pBoss, FVector a_vLoc, float a_fDamage)
{
	// Return if boss is dying or dead
	if (m_fHealth <= 0)
	{
		return;
	}

	// Deal damage
	a_pBoss->TakeDamage(a_fDamage);

	// Get boss' combat component
	UCombatComponent* pCombatComp = a_pBoss->GetCombatComponent();

	// Null check component
	if (pCombatComp)
	{
		// Spawn Blood Particle at the hit location
		if (pCombatComp->m_pImpactParticle != nullptr)
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), pCombatComp->m_pImpactParticle, a_vLoc, FQuat::Identity.Rotator());

		// Play damage indicator sound
		if (pCombatComp->m_pImpactSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), pCombatComp->m_pImpactSound, GetOwner()->GetActorLocation());
	}

	// Deplete stun meter
	a_pBoss->GetStatsComponenet()->DecrementStun(15.0f);
}

void UCombatComponent::HandlePlayerDamage(ASwordFightingGameCharacter* a_pPlayer, FVector a_vLoc, float a_fDamage, bool a_bKnockback)
{
	// Return if player is dying or dead
	if (m_fHealth <= 0)
	{
		return;
	}

	// Get player's evasion componenet
	UEvasionComponent* pEvasionComp = Cast<UEvasionComponent>(a_pPlayer->GetComponentByClass(UEvasionComponent::StaticClass()));

	// Don't deal damage if player is dodging, staggered, or already dying
	if (!pEvasionComp->m_bIsDodging && !IsStaggered() && !a_pPlayer->IsDying())
	{
		// Get player's combat componenet
		UCombatComponent* playerCombatComp = Cast<UCombatComponent>(a_pPlayer->GetComponentByClass(UCombatComponent::StaticClass()));

		// Return early if invalid
		if (!playerCombatComp)
			return;

		// Play hurt animation and deal damage
		if (playerCombatComp->m_pHurtMontage && !a_bKnockback)
		{
			// If player isn't block do normal damage
			if (!playerCombatComp->m_bIsBlocking)
			{
				// Play hurt sound
				if (playerCombatComp->m_pImpactSound)
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), playerCombatComp->m_pImpactSound, GetOwner()->GetActorLocation());

				a_pPlayer->GetMesh()->GetAnimInstance()->Montage_Play(playerCombatComp->m_pHurtMontage);
				a_pPlayer->TakeDamage(a_fDamage);
			}
			else // Otherwise negate some damage
			{
				// Play impact sound
				if (playerCombatComp->m_pBlockSound)
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), playerCombatComp->m_pBlockSound, GetOwner()->GetActorLocation());

				a_pPlayer->GetMesh()->GetAnimInstance()->Montage_Play(playerCombatComp->m_pShieldImpactMontage);
				a_pPlayer->TakeDamage(a_fDamage / 2.0f);
			}
		}

		// Handle Forceful attacks
		if (a_bKnockback)
		{
			// Spawn Blood Particle at the hit location
			if (playerCombatComp->m_pImpactParticle)
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), playerCombatComp->m_pImpactParticle, a_vLoc, FQuat::Identity.Rotator());

			// Initiate knockback
			Knockback(playerCombatComp);

			// Damage player
			a_pPlayer->TakeDamage(a_fDamage);
		}
	}
}

void UCombatComponent::HandleDamage(ACharacter* a_pVictim, FVector a_vLoc, FAttackStats a_sAttackStats)
{

}

void UCombatComponent::Attack(FString a_sAttackName)
{
	// Check if actor is alive
	if (m_fHealth <= 0.0f)
		return;

	// Check if actor is staggered
	if (IsStaggered())
		return;
	
	// Check if map has elements
	if (m_mAttackMap.Num() == 0)
		return;

	// Get owner of component and then get their animator
	UAnimInstance* pAnimInst = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();

	// Return early if the anim instance wasn't found
	if (!pAnimInst)
		return;

	// Get the anim montage to play
	UAnimMontage** pAttack = m_mAttackMap.Find(a_sAttackName);

	// Play the attack if valid
	if (pAttack)
		pAnimInst->Montage_Play(*pAttack);
}

void UCombatComponent::StopAttack(float a_fBlendOutTime, FString a_sAttackName)
{
	// Check if map has elements
	if (m_mAttackMap.Num() == 0)
		return;

	// Get owner of component and then get their animator
	UAnimInstance* pAnimInst = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();

	// Return early if the anim instance wasn't found
	if (!pAnimInst)
		return;

	// Get the anim montage to play
	UAnimMontage** pAttack = m_mAttackMap.Find(a_sAttackName);

	// Play the attack if valid
	if (pAttack)
		pAnimInst->Montage_Stop(a_fBlendOutTime, *pAttack);
}

// Returns whether or not actor is attacking
bool UCombatComponent::IsAttacking()
{
	// Check if map has elements
	if (m_mAttackMap.Num() == 0)
		return false;

	// Get owner of component and then get their animator
	UAnimInstance* pAnimInst = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();

	// Return early if the anim instance wasn't found
	if (!pAnimInst)
		return false;

	// Get map keys
	TArray<FString> aKeys;
	m_mAttackMap.GetKeys(aKeys);

	// Check all attacks to see if any of them are currently playing
	for (int i = 0; i < aKeys.Num(); i++)
	{
		// Return when you find an attack playing
		if (pAnimInst->Montage_IsPlaying(*m_mAttackMap.Find(aKeys[i])))
			return true;
	}

	return false;
}

void UCombatComponent::Knockback(UCombatComponent* a_pCombatComp)
{
	// Get owner of component and then get their animator
	UAnimInstance* pAnimInst = Cast<ACharacter>(a_pCombatComp->GetOwner())->GetMesh()->GetAnimInstance();
	ACharacter* pOwner = Cast<ACharacter>(a_pCombatComp->GetOwner());

	// Play damage indicator sound
	if (a_pCombatComp->m_pKnockbackSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), a_pCombatComp->m_pKnockbackSound, pOwner->GetActorLocation());

	if (!pAnimInst->Montage_IsPlaying(a_pCombatComp->m_pKnockbackMontage))
	{
		FVector playerToEnemy = pOwner->GetActorLocation() - GetOwner()->GetActorLocation();
		pOwner->LaunchCharacter(playerToEnemy * 30, true, true);
		pAnimInst->Montage_Play(a_pCombatComp->m_pKnockbackMontage);
	}
}

bool UCombatComponent::IsStaggered()
{
	// Get owner of component and then get their animator
	UAnimInstance* pAnimInst = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();

	// Return early if the anim instance wasn't found
	if (!pAnimInst)
		return false;

	// Check knockback animations if able
	if(m_bCanBeKnockedDown)
		return pAnimInst->Montage_IsPlaying(m_pHurtMontage) || pAnimInst->Montage_IsPlaying(m_pKnockbackMontage) || pAnimInst->Montage_IsPlaying(m_pGetUpMontage);

	// Check if any stagger animations are playing
	return pAnimInst->Montage_IsPlaying(m_pHurtMontage);
}

void UCombatComponent::Block()
{
	m_bIsBlocking = true;
}

void UCombatComponent::StopBlocking()
{
	m_bIsBlocking = false;
}


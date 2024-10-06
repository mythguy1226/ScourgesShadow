// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Boss.h"
#include "SwordFightingGameCharacter.h"
#include "EvasionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy.h"

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
			if (Cast<AEnemy>(i->GetActor()) && Cast<AEnemy>(GetOwner()))
				continue;

			// Try casting actor to a character
			ACharacter* pVictim = Cast<ACharacter>(i->GetActor());

			// Handle damage
			if (pVictim)
				HandleDamage(pVictim, a_vLocation, a_sAttackStats);
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
			if (Cast<AEnemy>(i->GetActor()) && Cast<AEnemy>(GetOwner()))
				continue;

			// Try casting actor to a character
			ACharacter* pVictim = Cast<ACharacter>(i->GetActor());

			// Handle damage
			if (pVictim)
				HandleDamage(pVictim, a_vEndLoc, a_sAttackStats);
		}
	}
}

void UCombatComponent::HandleDamage(ACharacter* a_pVictim, FVector a_vLoc, FAttackStats a_sAttackStats)
{
	// Get the character's combat component
	UCombatComponent* pVicCombComp = Cast<UCombatComponent>(a_pVictim->GetComponentByClass(UCombatComponent::StaticClass()));

	// Get the character's stats component
	UStatsComponent* pVicStatsComp = Cast<UStatsComponent>(a_pVictim->GetComponentByClass(UStatsComponent::StaticClass()));

	// Get the character's evasion component
	UEvasionComponent* pEvasionComp = Cast<UEvasionComponent>(a_pVictim->GetComponentByClass(UEvasionComponent::StaticClass()));

	// Return if victim doesnt have a combat component
	if (!pVicCombComp)
		return;

	// Return if player is dying or dead
	if (pVicCombComp->m_fHealth <= 0)
		return;

	// Return if dodging
	if (pEvasionComp)
		if (pEvasionComp->m_bIsDodging)
			return;

	// Return if actively dying
	if (pVicCombComp->IsDying())
		return;

	// Return if staggered by knockdown
	if (pVicCombComp->m_bCanBeKnockedDown && pVicCombComp->IsStaggered())
		return;

	// Handle blocked attacks
	if (pVicCombComp->m_pHurtMontage && !a_sAttackStats.sKnockback && pVicCombComp->m_bCanBlock)
	{
		// If player isn't block do normal damage
		if (!pVicCombComp->m_bIsBlocking)
		{
			// Play hurt sound
			if (pVicCombComp->m_pImpactSound)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), pVicCombComp->m_pImpactSound, GetOwner()->GetActorLocation());

			// Deplete stun meter
			pVicStatsComp->DecrementStun(a_sAttackStats.sStunDamage);

			// Deal damage
			pVicCombComp->TakeDamage(a_sAttackStats.sDamage);
		}
		else // Otherwise negate some damage
		{
			// Play impact sound
			if (pVicCombComp->m_pBlockSound)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), pVicCombComp->m_pBlockSound, GetOwner()->GetActorLocation());

			a_pVictim->GetMesh()->GetAnimInstance()->Montage_Play(pVicCombComp->m_pShieldImpactMontage);
			pVicCombComp->TakeDamage(a_sAttackStats.sDamage / 2.0f);
		}
	}
	if(!pVicCombComp->m_bCanBlock) // Handle attacks for those who cant block
	{
		// Spawn impact particle at the hit location
		if (pVicCombComp->m_pImpactParticle != nullptr)
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), pVicCombComp->m_pImpactParticle, a_vLoc, FQuat::Identity.Rotator());

		// Play damage indicator sound
		if (pVicCombComp->m_pImpactSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), pVicCombComp->m_pImpactSound, GetOwner()->GetActorLocation());

		// Deplete stun meter
		pVicStatsComp->DecrementStun(a_sAttackStats.sStunDamage);

		// Deal damage
		pVicCombComp->TakeDamage(a_sAttackStats.sDamage);
	}

	// Handle Forceful attacks
	if (a_sAttackStats.sKnockback)
	{
		// Knockback opponent
		Knockback(pVicCombComp);

		// Deal damage
		pVicCombComp->TakeDamage(a_sAttackStats.sDamage);
	}
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
	// Return if dying
	if (a_pCombatComp->IsDying())
		return;

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

bool UCombatComponent::IsDying()
{
	// Get owner of component and then get their animator
	UAnimInstance* pAnimInst = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();

	// Return early if the anim instance wasn't found
	if (!pAnimInst)
		return false;

	// Check if any dying animations are playing
	return pAnimInst->Montage_IsPlaying(m_pDeathMontage);
}

void UCombatComponent::Block()
{
	m_bIsBlocking = true;
}

void UCombatComponent::StopBlocking()
{
	m_bIsBlocking = false;
}

void UCombatComponent::TakeDamage(float a_fDamage)
{
	// Check if already dying
	if (IsDying())
		return;

	// Negate health by damage amount
	m_fHealth -= a_fDamage;

	// Handle death
	if (m_fHealth <= 0.0f)
		Die();
}

void UCombatComponent::Die()
{
	// Get owner of component and then get their animator
	UAnimInstance* pAnimInst = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();

	// Return early if the anim instance wasn't found
	if (!pAnimInst)
		return;

	// Play the death montage and death sound
	if (m_pDeathMontage)
		pAnimInst->Montage_Play(m_pDeathMontage);
	if(m_pDeathSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pDeathSound, GetOwner()->GetActorLocation());

	// Disable character movement
	Cast<ACharacter>(GetOwner())->GetCharacterMovement()->DisableMovement();
}

void UCombatComponent::InitiateHeal()
{
	// Get owner of component and then get their animator
	UAnimInstance* pAnimInst = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();

	// Return early if the anim instance wasn't found
	if (!pAnimInst)
		return;

	// Play the death montage and death sound
	if (m_pHealMontage)
		pAnimInst->Montage_Play(m_pHealMontage);
}

void UCombatComponent::Heal()
{
	// Increase health by amount and cap to max
	m_fHealth += m_fHealAmount;
	if (m_fHealth > m_fMaxHealth)
		m_fHealth = m_fMaxHealth;

	// Decrease number of heals
	m_iNumHeals--;
}

bool UCombatComponent::CanHeal()
{
	// Get evasive component to check if player tries to heal mid-dodge
	UEvasionComponent* pEvasiveComp = Cast<UEvasionComponent>(GetOwner()->GetComponentByClass(UEvasionComponent::StaticClass()));
	bool bDodging = false;
	if (pEvasiveComp)
		bDodging = pEvasiveComp->m_bIsDodging;

	// Returns whether or not player can heal
	return m_iNumHeals > 0 && !IsAttacking() && !IsStaggered() && !bDodging;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AttackStats.h"
#include "CombatComponent.generated.h"

class ASwordFightingGameCharacter;
class ABoss;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDFIGHTINGGAME_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Particles and sounds to pass in
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* m_pImpactParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* m_pImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* m_pKnockbackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* m_pBlockSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* m_pDeathSound;

	// Health meter
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fMaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float m_fHealth = m_fMaxHealth;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Damage handlers and trace generation handlers
	void GenerateHitSphere(FVector a_vLocation, float a_fRadius, FAttackStats a_sAttackStats, bool a_bDebug = false);
	void GenerateHitCapsule(FVector a_vBeginLoc, FVector a_vEndLoc, float a_fRadius, FAttackStats a_sAttackStats, bool a_bDebug = false);
	void HandleDamage(ACharacter* a_pVictim, FVector a_vLoc, FAttackStats a_sAttackStats);

	// Set of damaged actors that is tracked during combat
	TSet<AActor*> m_sDamagedActors;
	
	// Map that holds attack information
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UAnimMontage*> m_mAttackMap;

	// Method for executing attacks based on name
	void Attack(FString a_sAttackName);

	// Method for halting an attack early
	void StopAttack(float a_fBlendOutTime, FString a_sAttackName);
	
	// Check if actor is attacking
	bool IsAttacking();

	// Stagger montages
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pHurtMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pKnockbackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pGetUpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pDeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pShieldImpactMontage;

	// Method for handling knockback
	void Knockback(UCombatComponent* a_pCombatComp);

	// Methods for checking if staggered or dying
	bool IsStaggered();
	bool IsDying();

	// Indicates whether actor is able to be knocked down
	bool m_bCanBeKnockedDown = false;

	// Flag for blocking status
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool m_bIsBlocking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_bCanBlock = false;

	// Toggle blocking
	void Block();
	void StopBlocking();

	// Methods for taking damage and death
	void TakeDamage(float a_fDamage);
	void Die();

	// Methods for handling healing
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int m_iNumHeals = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fHealAmount = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pHealMontage;
	void InitiateHeal();
	void Heal();
	bool CanHeal();

};

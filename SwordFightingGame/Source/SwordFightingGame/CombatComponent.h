// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
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
	UNiagaraSystem* m_pImpactParticle;
	USoundBase* m_pImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fMaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float m_fHealth = m_fMaxHealth;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Damage handlers and trace generation handlers
	void GenerateHitSphere(FVector a_vLocation, float a_fRadius, float a_fDamage, bool a_bDebug = false, bool a_bKnockback = false);
	void GenerateHitCapsule(FVector a_vBeginLoc, FVector a_vEndLoc, float a_fRadius, float a_fDamage, bool a_bDebug = false, bool a_bKnockback = false);
	void HandleBossDamage(ABoss* a_pBoss, FVector a_vLoc, float a_fDamage);
	void HandlePlayerDamage(ASwordFightingGameCharacter* a_pPlayer, FVector a_vLoc, float a_fDamage, bool a_bKnockback);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pHurtMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* m_pDeathMontage;

	// Method for checking if staggered
	bool IsStaggered();
};

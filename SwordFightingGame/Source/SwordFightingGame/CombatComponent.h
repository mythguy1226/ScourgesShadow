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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float m_fHealth = 100.0f;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GenerateHitSphere(FVector a_vLocation, float a_fRadius, float a_fDamage, bool a_bDebug = false, bool a_bKnockback = false);
	void GenerateHitCapsule(FVector a_vBeginLoc, FVector a_vEndLoc, float a_fRadius, float a_fDamage, bool a_bDebug = false, bool a_bKnockback = false);
	void HandleBossDamage(ABoss* a_pBoss, FVector a_vLoc, float a_fDamage);
	void HandlePlayerDamage(ASwordFightingGameCharacter* a_pPlayer, FVector a_vLoc, float a_fDamage, bool a_bKnockback);
		
};

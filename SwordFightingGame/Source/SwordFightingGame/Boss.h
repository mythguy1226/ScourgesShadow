// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MoltenSword.h"
#include "Components/WidgetComponent.h"
#include "CombatComponent.h"
#include "BossStatsComponent.h"
#include "Boss.generated.h"

UCLASS()
class SWORDFIGHTINGGAME_API ABoss : public ACharacter
{
	GENERATED_BODY()

	// Reference to weapon class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> m_cSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UserInterface, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* m_pTargetLockWidget;

	// Combat Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* m_pCombatComponent;

	// Stats Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBossStatsComponent* m_pStatsComponent;

public:
	// Sets default values for this character's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// General Fields
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString m_sName = "Demonic Knight";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool m_bInCombat = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	FVector m_vDeathLocation;

	// Gets combat component
	UCombatComponent* GetCombatComponent();

	// Gets stat component
	UBossStatsComponent* GetStatsComponenet();

	// Combat methods
	void AttachSword();
	void SlashAttack();
	void KickAttack();
	void PoundAttack();
	AMoltenSword* m_pEquippedSword;

	// Reset method
	void Reset();
};

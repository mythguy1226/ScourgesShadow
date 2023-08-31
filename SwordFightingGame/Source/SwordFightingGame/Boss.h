// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MoltenSword.h"
#include "Components/WidgetComponent.h"
#include "Boss.generated.h"

UCLASS()
class SWORDFIGHTINGGAME_API ABoss : public ACharacter
{
	GENERATED_BODY()

	// Reference to weapon class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> m_cSword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UserInterface, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> m_cEnemyDeathScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UserInterface, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* m_pTargetLockWidget;

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

	// References to montages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pSlashAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pKickAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pPoundAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pDeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pHurtMontage;

	// References to sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<USoundBase*> m_pAttackSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<USoundBase*> m_pDeathSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pEnemyDamageSound;

	// General Fields
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString m_sName = "Demonic Knight";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool m_bInCombat = false;
	
	// Combat fields
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float m_fHealth = 300.0f;
	float m_fStunMeter = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	FVector m_vDeathLocation;

	// Combat methods
	void AttachSword();
	void SlashAttack();
	void KickAttack();
	void PoundAttack();
	void TakeDamage(float a_fDamage);
	AMoltenSword* m_pEquippedSword;
	FVector vStartPoint;
	FVector vMiddlePoint;
	FVector vEndPoint;
	void GenerateHitSphere(FVector a_vLocation, float a_fRadius, float a_fDamage, bool a_bDebug = false, bool a_bKnockback = false);

	// Death handling
	void Die();

	// Montage End Event Handler
	UFUNCTION()
	void HandleOnMontageEnded(UAnimMontage* a_pMontage, bool a_bInterrupted);

	UFUNCTION()
	void HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
};

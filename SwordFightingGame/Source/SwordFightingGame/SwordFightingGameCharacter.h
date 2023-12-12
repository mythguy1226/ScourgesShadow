// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Boss.h"
#include "NiagaraFunctionLibrary.h"
#include "CombatComponent.h"
#include "PlayerStatsComponent.h"
#include "SwordFightingGameCharacter.generated.h"

UCLASS(config=Game)
class ASwordFightingGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// Combat Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* m_pCombatComponent;

	// Stats Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UPlayerStatsComponent* m_pStatsComponent;

	// Reference to HUD class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInterface", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> m_cPlayerHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInterface", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> m_cPlayerDeathScreen;

public:
	ASwordFightingGameCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	// Player Anim Montages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pJumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pDeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pDodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pFallDodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pShieldImpactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pLockDodgeForwardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pLockDodgeRightMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pLockDodgeLeftMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pLockDodgeBackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pLockDodgeForwardLMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pLockDodgeForwardRMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pLockDodgeBackLMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* m_pLockDodgeBackRMontage;

	// Particle Systems
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UNiagaraSystem* m_pBloodParticle;

	// Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pSlashImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pKnockbackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pHurtSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pBlockSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pDeathSound;

	// Method for handling light attacks
	void LightAttack();
	void HeavyAttack();

	// Method for getting whether player is attacking
	int m_iComboIndex = 0;
	bool m_bIsCharging = false;
	bool m_bIsCharged = false;

	// *** Handle Sprinting ***
	// Methods for Sprint Handling
	void Sprint();
	void StopSprinting();
	bool m_bIsSprinting = false;

	// *** Handle Blocking ***
	// Methods for Block Handling
	void Block();
	void StopBlocking();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool m_bIsBlocking = false;

	// *** Handle Dodging ***
	void Dodge();
	bool m_bIsDodging = false;
	bool m_bCanAttackAfterDodge = false;
	bool m_bAttackingAfterDodge = false;
	FVector dodgeDir;

	// *** Handle Target Locking ***
	void ToggleTargetLock();
	ABoss* m_pTargetLockedBoss;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool m_bIsTargetLocked = false;

	// *** Handle Damage and Staggering ***
	void TakeDamage(float a_fDamage);
	bool IsDying();

protected:
	// Called upon begin of play
	virtual void BeginPlay();

	// Called upon Tick
	virtual void Tick(float a_fDeltaTime);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	// Overriding Jump Method
	void Jump() override;

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	UFUNCTION()
	void HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void HandleOnMontageEnd(UAnimMontage* a_pMontage, bool a_bInterrupted);

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
private:
	// Perception Stimulus Component
	class UAIPerceptionStimuliSourceComponent* m_pStimulus;

	// Method for setting up perception Stimulus
	void SetupStimulus();
};


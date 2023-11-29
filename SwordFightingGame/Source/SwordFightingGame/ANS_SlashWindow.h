// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_SlashWindow.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API UANS_SlashWindow : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	// Fields used for location tracking
	FVector vSlashBegin;
	FVector vSlashEnd;

	// Fields for tracking timing of checks
	float fTimeBetweenChecks = 0.001f;
	float fCheckTimer = 0.001f;

public:

	// Socket used for hurt box generation
	UPROPERTY(EditAnywhere)
	FString SocketName;

	// Radius of hit sphere or capsule
	UPROPERTY(EditAnywhere)
	float m_fRadius;

	// Damage dealt
	UPROPERTY(EditAnywhere)
	float m_fDamage;

	// Whether or not to use continuous slash tracking or single check
	UPROPERTY(EditAnywhere)
	bool m_bContinuousTracing = true;

	// Override firing in editor to be false
	bool bShouldFireInEditor = false;

	// Override method for handling beginning of notify
	UFUNCTION()
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
	// Override method for handling tick of notify
	UFUNCTION()
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	// Override method for handling end of notify
	UFUNCTION()
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

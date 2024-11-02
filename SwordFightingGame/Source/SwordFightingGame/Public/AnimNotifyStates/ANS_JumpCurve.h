// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Curves/CurveVector.h"
#include "ANS_JumpCurve.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API UANS_JumpCurve : public UAnimNotifyState
{
	GENERATED_BODY()
private:
	float m_fDuration;
public:
	// Reference to curve
	UPROPERTY(EditAnywhere)
	UCurveVector* m_pCurve;

	// Length values
	UPROPERTY(EditAnywhere)
	float m_fHeight;

	UPROPERTY(EditAnywhere)
	float m_fForward;

	UPROPERTY(EditAnywhere)
	float m_fRightward;

	// Override method for handling beginning of notify
	UFUNCTION()
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	// Override method for handling tick of notify
	UFUNCTION()
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
};

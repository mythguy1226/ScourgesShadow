// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Blueprint/UserWidget.h"
#include "AN_PlayWidgetAnim.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API UAN_PlayWidgetAnim : public UAnimNotify
{
	GENERATED_BODY()

	// Get widget class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> m_cWidgetClass;

public:
	// Name of the animation
	UPROPERTY(EditAnywhere)
	FString m_sAnimationName;

	// Flag for reverse animations
	UPROPERTY(EditAnywhere)
	bool m_bReversed = false;

	// Override method for handling notify
	UFUNCTION()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_HealPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API UAN_HealPlayer : public UAnimNotify
{
	GENERATED_BODY()
public:
	// Override method for handling notify
	UFUNCTION()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

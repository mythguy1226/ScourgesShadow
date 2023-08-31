// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GlobalManager.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API UGlobalManager : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void PlayWidgetAnimation(UUserWidget* a_pWidget, FString a_sName, bool a_bReverse = false);
	void PlayDelayedWidgetAnimation(UUserWidget* a_pWidget, FString a_sName, bool a_bReverse = false, float a_fDelay = 1.0f);
};

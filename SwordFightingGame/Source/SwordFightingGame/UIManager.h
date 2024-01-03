// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlobalService.h"
#include "UIManager.generated.h"

UCLASS()
class SWORDFIGHTINGGAME_API AUIManager : public AGlobalService
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUIManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Methods used for playing widget animations
	void PlayWidgetAnimation(UUserWidget* a_pWidget, FString a_sName, bool a_bReverse = false);
	void PlayDelayedWidgetAnimation(UUserWidget* a_pWidget, FString a_sName, bool a_bReverse = false, float a_fDelay = 1.0f);
};

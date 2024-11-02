// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "General/GlobalService.h"
#include "AudioManager.generated.h"

UCLASS()
class SWORDFIGHTINGGAME_API AAudioManager : public AGlobalService
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudioManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pDungeonAmbience;
	UAudioComponent* m_pDungeonAmbienceComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* m_pBossMusic;
	UAudioComponent* m_pBossMusicComp;

	void StartAmbience();
	void StopAmbience();

	void StartBossMusic();
	void StopBossMusic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

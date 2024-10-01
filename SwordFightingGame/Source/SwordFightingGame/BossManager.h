// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "BossManager.generated.h"

UCLASS()
class SWORDFIGHTINGGAME_API ABossManager : public AActor
{
	GENERATED_BODY()

	/* Get the blueprint widget to assign to the player viewport */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> m_cBossBarUI;
	
public:	
	// Sets default values for this actor's properties
	ABossManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* m_pActiveBoss = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString m_sActiveBossName = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool m_bFightActive = false;

	UUserWidget* m_pBossBar = nullptr;

	void StartBossFight();
	void EndBossFight();

};

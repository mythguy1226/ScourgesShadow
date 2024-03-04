// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatComponent.h"
#include "StatsComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemy.generated.h"

UCLASS()
class SWORDFIGHTINGGAME_API AEnemy : public ACharacter
{
	GENERATED_BODY()

	// Combat Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* m_pCombatComponent;

	// Stats Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStatsComponent* m_pStatsComponent;

	// Widget Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UserInterface, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* m_pHealthBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UserInterface, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* m_pTargetLockWidget;

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Combat tracker
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool m_bInCombat = false;

	// Reference to where enemy should go when killed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	FVector m_vDeathLocation;

	// Method used for resets after being killed
	void Reset();
};

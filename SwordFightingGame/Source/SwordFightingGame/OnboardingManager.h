// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Instructions.h"
#include "Blueprint/UserWidget.h"
#include "OnboardingManager.generated.h"

UCLASS()
class SWORDFIGHTINGGAME_API AOnboardingManager : public AActor
{
	GENERATED_BODY()

	// Reference to instructions UI class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInterface", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> m_cInstructionsScreen;
	
public:	
	// Sets default values for this actor's properties
	AOnboardingManager();

	// Editable array of instructions
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInstructions> m_aInstructions;

	// Current index in array to display
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int m_iInstructionIndex = 0;

	// Whether or not to display instructions
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_bShowInstructions = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

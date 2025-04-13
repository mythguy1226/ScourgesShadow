// Fill out your copyright notice in the Description page of Project Settings.


#include "General/UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUIManager::AUIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUIManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUIManager::PlayWidgetAnimation(UUserWidget* a_pWidget, FString a_sName, bool a_bReverse)
{
	// Return if the widget is null
	if (a_pWidget)
	{
		return;
	}

	// Variable to store widget anim
	UWidgetAnimation* pAnim = nullptr;

	// Get blueprint generated class of widget and check if its valid
	UWidgetBlueprintGeneratedClass* WidgetClass = Cast<UWidgetBlueprintGeneratedClass>(a_pWidget->GetClass());
	if (WidgetClass)
	{
		// Iterate through all widget animations attached
		for (UWidgetAnimation* Animation : WidgetClass->Animations)
		{
			// If the names match what we're looking for, set the animation to current
			if (Animation && Animation->GetFName() == a_sName)
			{
				pAnim = Animation;
			}
		}
	}

	// Play the widget animation if valid
	if (pAnim != nullptr)
	{
		// Play the animation backwards
		if (a_bReverse)
		{
			a_pWidget->AddToViewport(0);
			a_pWidget->PlayAnimation(pAnim, pAnim->GetEndTime(), 1, EUMGSequencePlayMode::Reverse);
		}
		else // Play the animation normally
		{
			a_pWidget->AddToViewport(0);
			a_pWidget->PlayAnimation(pAnim);
		}
	}
}

void AUIManager::PlayDelayedWidgetAnimation(UUserWidget* a_pWidget, FString a_sName, bool a_bReverse, float a_fDelay)
{
	// Set a timer for playing the widget animation
	FTimerHandle pTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(pTimerHandle, [&]()
	{
		// Play the widget animation
		PlayWidgetAnimation(a_pWidget, a_sName, a_bReverse);

	}, a_fDelay, false);
}


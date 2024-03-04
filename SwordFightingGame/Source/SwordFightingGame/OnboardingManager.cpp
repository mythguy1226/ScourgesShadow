// Fill out your copyright notice in the Description page of Project Settings.


#include "OnboardingManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOnboardingManager::AOnboardingManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOnboardingManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Attach screen to player
	if (m_cInstructionsScreen)
	{
		// Add the HUD to our viewport
		UUserWidget* HUD = CreateWidget<UUserWidget>(Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)), m_cInstructionsScreen);
		HUD->AddToViewport(9999);
	}
}

// Called every frame
void AOnboardingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


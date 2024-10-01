// Fill out your copyright notice in the Description page of Project Settings.


#include "BossManager.h"
#include "Boss_Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Boss.h"
#include "GlobalManager.h"
#include "AudioManager.h"

// Sets default values
ABossManager::ABossManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABossManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossManager::StartBossFight()
{
	// Get the active boss' controller and cast to an enemy controller
	if (m_pActiveBoss)
	{
		ABoss_Controller* pController = Cast<ABoss_Controller>(Cast<ACharacter>(m_pActiveBoss)->GetController());
		if (pController)
		{
			// Enable controller activity
			pController->SetActive(true);
			m_bFightActive = true;

			// Assign the Boss Bar UI to player viewport
			if (m_cBossBarUI)
			{
				// Get reference to player controller
				APlayerController* pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

				// Get the widget and add it to the viewport
				m_pBossBar = CreateWidget<UUserWidget>(pPlayerController, m_cBossBarUI);
				m_pBossBar->AddToViewport(9999);
			}
		}

		ABoss* pBoss = Cast<ABoss>(pController->GetPawn());

		// Check for initial combat status
		if (!pBoss->m_bInCombat)
		{
			pBoss->m_bInCombat = true;

			// Get the audio manager and stop the ambience to play the boss music
			AAudioManager* pAudioMngr = Cast<AAudioManager>(
				Cast<UGlobalManager>(
					UGameplayStatics::GetGameInstance(GetWorld()))->GetService<AAudioManager>()
			);
			if (!pAudioMngr)
				return;

			pAudioMngr->StopAmbience();
			pAudioMngr->StartBossMusic();
		}
	}
}

void ABossManager::EndBossFight()
{
	// Clean up references
	m_pActiveBoss = nullptr;
	m_bFightActive = false;

	// Remove boss bar from viewport
	m_pBossBar->RemoveFromViewport();
}


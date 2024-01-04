// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAudioManager::AAudioManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AAudioManager::StartAmbience()
{
	if(m_pDungeonAmbienceComp)
		m_pDungeonAmbienceComp->Play();
}

void AAudioManager::StopAmbience()
{
	if (m_pDungeonAmbienceComp)
		m_pDungeonAmbienceComp->Stop();
}

void AAudioManager::StartBossMusic()
{
	if(m_pBossMusicComp)
		m_pBossMusicComp->Play();
}

void AAudioManager::StopBossMusic()
{
	if (m_pBossMusicComp)
		m_pBossMusicComp->Stop();
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the audio components
	m_pDungeonAmbienceComp = UGameplayStatics::CreateSound2D(GetWorld(), m_pDungeonAmbience);
	m_pBossMusicComp = UGameplayStatics::CreateSound2D(GetWorld(), m_pBossMusic);
	
	// Play dungeon ambience
	StartAmbience();
}

// Called every frame
void AAudioManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


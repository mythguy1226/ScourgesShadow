// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossTrigger.h"
#include "AI/BossManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossTrigger::ABossTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up box component for trigger collider
	m_pBoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Collider"));
	m_pBoxComponent->InitBoxExtent(FVector(100.0f));
	m_pBoxComponent->SetupAttachment(RootComponent);

	// Register overlap events for box component
	m_pBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossTrigger::OnBoxBeginOverlap);
}

// Called when the game starts or when spawned
void ABossTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossTrigger::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check that other actor and boss selector have valid references
	if (OtherActor && m_pBoss)
	{
		// Get reference to boss manager
		ABossManager* pBossMngr = Cast<ABossManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossManager::StaticClass()));

		// If not already in a fight, start the boss fight with the selected boss
		if (!pBossMngr->m_bFightActive)
		{
			pBossMngr->m_pActiveBoss = m_pBoss;
			pBossMngr->m_sActiveBossName = m_sName;
			pBossMngr->StartBossFight();
		}
	}
}


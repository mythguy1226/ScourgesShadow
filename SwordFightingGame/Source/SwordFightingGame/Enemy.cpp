// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "SwordFightingGameCharacter.h"
#include "TargetLockComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the target-lock widget
	m_pHealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	m_pHealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	m_pHealthBarWidget->SetupAttachment(RootComponent);

	// Set up the target-lock widget
	m_pTargetLockWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Target Point"));
	m_pTargetLockWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 25.0f));
	m_pTargetLockWidget->SetupAttachment(RootComponent);

	// Set up the combat component
	m_pCombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	m_pStatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Reset()
{
	// Get the player's target lock component and disable target locking
	ASwordFightingGameCharacter* pPlayer = Cast<ASwordFightingGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Cast<UTargetLockComponent>(pPlayer->GetComponentByClass(UTargetLockComponent::StaticClass()))->DisableLock();

	// Update combat status
	m_bInCombat = false;

	// Remove actor from arena
	TeleportTo(m_vDeathLocation, GetActorRotation());
}


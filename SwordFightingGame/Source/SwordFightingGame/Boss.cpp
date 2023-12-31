// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "SwordFightingGameCharacter.h"
#include "AudioManager.h"
#include "Blueprint/UserWidget.h"
#include "GlobalManager.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the target-lock widget
	m_pTargetLockWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Target Point"));
	m_pTargetLockWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	m_pTargetLockWidget->SetupAttachment(RootComponent);

	// Set up the combat component
	m_pCombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	m_pStatsComponent = CreateDefaultSubobject<UBossStatsComponent>(TEXT("Stats Component"));
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	// Super call
	Super::BeginPlay();
	
	// Bind events
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ABoss::HandleOnMontageEnded);

	// Attach the sword to the boss 
	AttachSword();
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get the player
	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Calculate Rotation for the widget and set its rotation
	FRotator rWidgetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), pPlayer->GetActorLocation());
	m_pTargetLockWidget->SetWorldRotation(rWidgetRot);

}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UCombatComponent* ABoss::GetCombatComponent()
{
	return m_pCombatComponent;
}

UBossStatsComponent* ABoss::GetStatsComponenet()
{
	return m_pStatsComponent;
}

void ABoss::AttachSword()
{
	// Null check the sword class
	if (m_cSword != nullptr)
	{
		// From Controller get the spawn location and rotation
		const FRotator pRotation = GetActorRotation();
		const FVector pLocation = GetActorLocation();

		// Define spawn params to always spawn regardless of collision
		FActorSpawnParameters pSpawnParams;
		pSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the sword into the world
		AActor* pSword = GetWorld()->SpawnActor<AMoltenSword>(m_cSword, pLocation, pRotation, pSpawnParams);

		m_pEquippedSword = Cast<AMoltenSword>(pSword);

		// Attach the sword
		FAttachmentTransformRules sAttachmentRules(EAttachmentRule::SnapToTarget, true);
		pSword->AttachToComponent(GetMesh(), sAttachmentRules, FName("SwordSocket"));
	}
}

void ABoss::SlashAttack()
{
	// Play attack montage
	m_pCombatComponent->Attack("SlashAttack");
}

void ABoss::KickAttack()
{
	// Play attack montage
	m_pCombatComponent->Attack("KickAttack");
}

void ABoss::PoundAttack()
{
	// Play attack montage
	m_pCombatComponent->Attack("PoundAttack");
}

void ABoss::HandleOnMontageEnded(UAnimMontage* a_pMontage, bool a_bInterrupted)
{
	// Check for death montage
	if (a_pMontage->GetName().Contains("Death_Montage"))
	{
		// Update combat status
		m_bInCombat = false;

		// Play the widget animation for fading in
		if (m_cEnemyDeathScreen != nullptr)
		{
			// Get the death screen
			UUserWidget* pDeathScreen = CreateWidget<UUserWidget>(Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)), m_cEnemyDeathScreen);
			
			// Play the death screen widget animation
			Cast<UGlobalManager>(UGameplayStatics::GetGameInstance(GetWorld()))->PlayWidgetAnimation(pDeathScreen, "Fade");


			// Set timer for dodge attacking
			FTimerHandle pTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(pTimerHandle, [&]()
			{
				// Stop the boss music and go back to playing ambient music
				AAudioManager* pAudioMngr = Cast<AAudioManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AAudioManager::StaticClass()));
				pAudioMngr->StopBossMusic();
				pAudioMngr->StartAmbience();

				// Remove actor from arena
				TeleportTo(m_vDeathLocation, GetActorRotation());

				// END of executed code

			}, 3, false);
		}
	}
}

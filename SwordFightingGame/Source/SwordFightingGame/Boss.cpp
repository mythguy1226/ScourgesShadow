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

	m_pTargetLockWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Target Point"));
	m_pTargetLockWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	m_pTargetLockWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	// Super call
	Super::BeginPlay();
	
	// Bind events
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ABoss::HandleOnMontageEnded);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &ABoss::HandleOnMontageNotifyBegin);

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
	UAnimInstance* pAnimInstance = GetMesh()->GetAnimInstance();
	if (pAnimInstance != nullptr)
	{
		// Can't attack when staggered
		if (pAnimInstance->Montage_IsPlaying(m_pHurtMontage) || m_fHealth <= 0.0f)
		{
			return;
		}

		// Play attack montage
		if (m_pSlashAttackMontage != nullptr)
		{
			pAnimInstance->Montage_Play(m_pSlashAttackMontage);

			// Play attack sounds
			if (m_pAttackSounds.Num() > 0)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pAttackSounds[FMath::RandRange(0, m_pAttackSounds.Num() - 1)], GetActorLocation());
			}
		}
	}
}

void ABoss::KickAttack()
{
	UAnimInstance* pAnimInstance = GetMesh()->GetAnimInstance();
	if (pAnimInstance != nullptr)
	{
		// Can't attack when staggered
		if (pAnimInstance->Montage_IsPlaying(m_pHurtMontage) || m_fHealth <= 0.0f)
		{
			return;
		}

		// Play attack montage
		if (m_pKickAttackMontage != nullptr)
		{
			pAnimInstance->Montage_Play(m_pKickAttackMontage);
		}
	}
}

void ABoss::PoundAttack()
{
	UAnimInstance* pAnimInstance = GetMesh()->GetAnimInstance();
	if (pAnimInstance != nullptr)
	{
		// Can't attack when staggered
		if (pAnimInstance->Montage_IsPlaying(m_pHurtMontage) || m_fHealth <= 0.0f)
		{
			return;
		}

		// Play attack montage
		if (m_pPoundAttackMontage != nullptr)
		{
			pAnimInstance->Montage_Play(m_pPoundAttackMontage);
		}
	}
}

void ABoss::TakeDamage(float a_fDamage)
{
	// Check if death montage is playing
	UAnimInstance* pAnimInstance = GetMesh()->GetAnimInstance();
	if (pAnimInstance != nullptr)
	{
		if (pAnimInstance->Montage_IsPlaying(m_pDeathMontage))
		{
			return;
		}
	}

	// Play damage indicator
	if (m_pEnemyDamageSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pEnemyDamageSound, GetActorLocation());
	}

	// Deal damage
	m_fHealth -= a_fDamage;

	// Death handling
	if (m_fHealth <= 0)
	{
		Die();
	}
}

void ABoss::GenerateHitSphere(FVector a_vLocation, float a_fRadius, float a_fDamage, bool a_bDebug, bool a_bKnockback)
{
	// Init hit results
	TArray<FHitResult> outHits;
	TArray<AActor*> ignoredActors;
	ignoredActors.Add(this);

	// Use Sphere Shape
	FCollisionShape sphereShape;
	sphereShape.ShapeType = ECollisionShape::Sphere;
	sphereShape.SetSphere(a_fRadius);

	// Set the query params
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(this);

	// Sweep Trace at typical contact point of swing
	GetWorld()->SweepMultiByChannel(outHits, a_vLocation, a_vLocation, FQuat::Identity, ECollisionChannel::ECC_Pawn, sphereShape, queryParams);

	// Debug Trace to show visual representation
	if (a_bDebug)
	{
		// Sweep Trace at typical contact point of swing with debugging visuals
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), a_vLocation, a_vLocation,
			a_fRadius,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn), true, ignoredActors,
			EDrawDebugTrace::ForDuration, outHits, true, FColor::Red);
	}

	// Check that a collision happened
	if (outHits.Num() > 0)
	{
		// Iterate through the hit results
		for (auto i = outHits.CreateIterator(); i; i++)
		{
			// Try to cast to the player
			ASwordFightingGameCharacter* pPlayer = Cast<ASwordFightingGameCharacter>(i->GetActor());
			if (pPlayer) // Continue if valid
			{
				// Return if player is dying or dead
				if (pPlayer->m_fHealth <= 0)
				{
					return;
				}

				// Player can't be dodging
				if (!pPlayer->m_bIsDodging && !pPlayer->IsStaggered() && !pPlayer->IsDying())
				{
					// Play hurt animation and deal damage
					if (pPlayer->m_pHurtMontage != nullptr && !a_bKnockback)
					{
						// If player isn't block do normal damage
						if (!pPlayer->m_bIsBlocking)
						{
							// Play hurt sound
							if (pPlayer->m_pHurtSound != nullptr)
								UGameplayStatics::PlaySoundAtLocation(GetWorld(), pPlayer->m_pHurtSound, GetActorLocation());

							pPlayer->GetMesh()->GetAnimInstance()->Montage_Play(pPlayer->m_pHurtMontage);
							pPlayer->TakeDamage(a_fDamage);
						}
						else // Otherwise negate some damage
						{
							// Play impact sound
							if (pPlayer->m_pBlockSound != nullptr)
								UGameplayStatics::PlaySoundAtLocation(GetWorld(), pPlayer->m_pBlockSound, GetActorLocation());

							pPlayer->GetMesh()->GetAnimInstance()->Montage_Play(pPlayer->m_pShieldImpactMontage);
							pPlayer->TakeDamage(a_fDamage / 2.0f);
						}
					}

					// Handle Forceful attacks
					if (a_bKnockback)
					{
						// Spawn Blood Particle at the hit location
						if (pPlayer->m_pBloodParticle != nullptr)
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), pPlayer->m_pBloodParticle, i->Location, FQuat::Identity.Rotator());

						// Play damage indicator sound
						if (pPlayer->m_pKnockbackSound != nullptr)
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), pPlayer->m_pKnockbackSound, pPlayer->GetActorLocation());

						if (!pPlayer->GetMesh()->GetAnimInstance()->Montage_IsPlaying(pPlayer->m_pKnockbackMontage))
						{
							FVector playerToEnemy = pPlayer->GetActorLocation() - GetActorLocation();
							pPlayer->LaunchCharacter(playerToEnemy * 30, true, true);
							pPlayer->GetMesh()->GetAnimInstance()->Montage_Play(pPlayer->m_pKnockbackMontage);
							pPlayer->TakeDamage(a_fDamage);
						}
					}
				}
				break;
			}
		}
	}
}

void ABoss::Die()
{
	// Play death sounds
	if (m_pDeathSounds.Num() > 0)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_pDeathSounds[FMath::RandRange(0, m_pDeathSounds.Num() - 1)], GetActorLocation());
	}

	// Play death montage
	UAnimInstance* pAnimInstance = GetMesh()->GetAnimInstance();
	if (pAnimInstance != nullptr)
	{
		if (m_pDeathMontage != nullptr)
		{
			pAnimInstance->Montage_Play(m_pDeathMontage);
		}
	}

	// Disable movement
	GetCharacterMovement()->DisableMovement();
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

void ABoss::HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName.ToString() == "KickAttack")
	{
		// Sphere Location
		FVector vOffset = (GetActorForwardVector() * 80.0f) + FVector(0.0f, 0.0f, 25.0f);
		FVector pLocation = GetActorLocation() + vOffset;

		// Generate a hit sphere at the slash location
		GenerateHitSphere(pLocation, 30.0f * GetMesh()->GetComponentScale().Z, 10.0f);
	}
	if (NotifyName.ToString() == "PoundAttack")
	{
		// Sphere Location
		FVector vOffset = (GetActorForwardVector() * 130.0f) + FVector(0.0f, 0.0f, -90.0f);
		FVector pLocation = GetActorLocation() + vOffset;

		// Generate a hit sphere at the slash location
		GenerateHitSphere(pLocation, 200.0f * GetMesh()->GetComponentScale().Z, 30.0f, false, true);
	}

	if (NotifyName.ToString().Contains("Start"))
	{
		// Set the start point
		USceneComponent* pTracker = Cast<USceneComponent>(m_pEquippedSword->GetComponentByClass(USceneComponent::StaticClass()));
		vStartPoint = pTracker->GetComponentLocation() + FVector(0.0f, 0.0f, -40.0f);
	}
	if (NotifyName.ToString().Contains("Middle"))
	{
		// Set the middle point
		USceneComponent* pTracker = Cast<USceneComponent>(m_pEquippedSword->GetComponentByClass(USceneComponent::StaticClass()));
		vMiddlePoint = pTracker->GetComponentLocation() + FVector(0.0f, 0.0f, -40.0f);

		// Init hit results
		TArray<FHitResult> outHits;
		TArray<AActor*> ignoredActors;
		ignoredActors.Add(this);

		// Use Capsule Shape
		FCollisionShape sphereShape;
		sphereShape.ShapeType = ECollisionShape::Sphere;
		sphereShape.SetSphere(30.0f * GetMesh()->GetComponentScale().Z);

		// Set the query params
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(this);
		
		// Sweep Trace between the start and middle points of the swing
		GetWorld()->SweepMultiByChannel(outHits, vStartPoint, vMiddlePoint, FQuat::Identity, ECollisionChannel::ECC_Pawn, sphereShape, queryParams);
		// \/   LEAVE THIS FOR DEBUGGING!!!   \/
		//UKismetSystemLibrary::SphereTraceMulti(GetWorld(), vStartPoint, vMiddlePoint, 
		//	30.0f * GetMesh()->GetComponentScale().Z,
		//	UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn), true, ignoredActors,
		//	EDrawDebugTrace::ForDuration, outHits, true, FColor::Red);

		// Check that a collision happened
		if (outHits.Num() > 0)
		{
			// Iterate through the hit results
			for (auto i = outHits.CreateIterator(); i; i++)
			{
				// Try to cast to the player
				ASwordFightingGameCharacter* pPlayer = Cast<ASwordFightingGameCharacter>(i->GetActor());
				if (pPlayer) // Continue if valid
				{
					// Return if player is dying or dead
					if (pPlayer->m_fHealth <= 0)
					{
						return;
					}

					// Don't deal damage if player is dodging
					if (!pPlayer->m_bIsDodging && !pPlayer->IsStaggered() && !pPlayer->IsDying())
					{
						// Spawn Blood Particle at the hit location
						if (pPlayer->m_pBloodParticle != nullptr)
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), pPlayer->m_pBloodParticle, i->Location, FQuat::Identity.Rotator());

						// Play hurt animation and deal damage
						if (pPlayer->m_pKnockbackMontage != nullptr)
						{
							if (!pPlayer->GetMesh()->GetAnimInstance()->Montage_IsPlaying(pPlayer->m_pKnockbackMontage))
							{
								FVector playerToEnemy = pPlayer->GetActorLocation() - GetActorLocation();
								pPlayer->LaunchCharacter(playerToEnemy * 30, true, true);
								pPlayer->GetMesh()->GetAnimInstance()->Montage_Play(pPlayer->m_pKnockbackMontage);
								pPlayer->TakeDamage(20.0f);

								// Play damage indicator sound
								if (pPlayer->m_pKnockbackSound != nullptr)
									UGameplayStatics::PlaySoundAtLocation(GetWorld(), pPlayer->m_pKnockbackSound, pPlayer->GetActorLocation());
							}
						}
					}
					break;
				}
			}
		}
	}
	if (NotifyName.ToString().Contains("End"))
	{
		// Set the end point
		USceneComponent* pTracker = Cast<USceneComponent>(m_pEquippedSword->GetComponentByClass(USceneComponent::StaticClass()));
		vEndPoint = pTracker->GetComponentLocation() + FVector(0.0f, 0.0f, -40.0f);

		// Init hit results
		TArray<FHitResult> outHits;
		TArray<AActor*> ignoredActors;
		ignoredActors.Add(this);

		// Use Capsule Shape
		FCollisionShape sphereShape;
		sphereShape.ShapeType = ECollisionShape::Sphere;
		sphereShape.SetSphere(30.0f * GetMesh()->GetComponentScale().Z);

		// Set the query params
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(this);

		// Sweep Trace between the start and middle points of the swing
		GetWorld()->SweepMultiByChannel(outHits, vMiddlePoint, vEndPoint, FQuat::Identity, ECollisionChannel::ECC_Pawn, sphereShape, queryParams);
		// \/   LEAVE THIS FOR DEBUGGING!!!   \/
		//UKismetSystemLibrary::SphereTraceMulti(GetWorld(), vMiddlePoint, vEndPoint, 
		//	30.0f * GetMesh()->GetComponentScale().Z,
		//	UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn), true, ignoredActors,
		//	EDrawDebugTrace::ForDuration, outHits, true, FColor::Red);

		// Check that a collision happened
		if (outHits.Num() > 0)
		{
			// Iterate through the hit results
			for (auto i = outHits.CreateIterator(); i; i++)
			{
				// Try to cast to the player
				ASwordFightingGameCharacter* pPlayer = Cast<ASwordFightingGameCharacter>(i->GetActor());
				if (pPlayer) // Continue if valid
				{
					// Return if player is dying or dead
					if (pPlayer->m_fHealth <= 0)
					{
						return;
					}

					// Don't deal damage if player is dodging
					if (!pPlayer->m_bIsDodging && !pPlayer->IsStaggered() && !pPlayer->IsDying())
					{
						// Play hurt animation and deal damage
						if (pPlayer->m_pKnockbackMontage != nullptr)
						{
							// Spawn Blood Particle at the hit location
							if (pPlayer->m_pBloodParticle != nullptr)
								UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), pPlayer->m_pBloodParticle, i->Location, FQuat::Identity.Rotator());

							if (!pPlayer->GetMesh()->GetAnimInstance()->Montage_IsPlaying(pPlayer->m_pKnockbackMontage))
							{
								FVector playerToEnemy = pPlayer->GetActorLocation() - GetActorLocation();
								pPlayer->LaunchCharacter(playerToEnemy * 30, true, true);
								pPlayer->GetMesh()->GetAnimInstance()->Montage_Play(pPlayer->m_pKnockbackMontage);
								pPlayer->TakeDamage(20.0f);

								// Play damage indicator sound
								if (pPlayer->m_pKnockbackSound != nullptr)
									UGameplayStatics::PlaySoundAtLocation(GetWorld(), pPlayer->m_pKnockbackSound, pPlayer->GetActorLocation());
							}
						}
					}
					break;
				}
			}
		}
	}
}


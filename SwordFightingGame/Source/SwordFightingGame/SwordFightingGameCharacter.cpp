// Copyright Epic Games, Inc. All Rights Reserved.

#include "SwordFightingGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Boss.h"
#include "Blueprint/UserWidget.h"
#include "GlobalManager.h"

//////////////////////////////////////////////////////////////////////////
// ASwordFightingGameCharacter

ASwordFightingGameCharacter::ASwordFightingGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Setup the player's Components
	m_pCombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	m_pCombatComponent->m_bCanBeKnockedDown = true;

	m_pStatsComponent = CreateDefaultSubobject<UPlayerStatsComponent>(TEXT("Stats Component"));

	m_pEvasionComponent = CreateDefaultSubobject<UEvasionComponent>(TEXT("Evasion Component"));

	SetupStimulus();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASwordFightingGameCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	m_bIsSprinting = true;
}

void ASwordFightingGameCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	m_bIsSprinting = false;
}

void ASwordFightingGameCharacter::Block()
{
	m_pCombatComponent->Block();
}

void ASwordFightingGameCharacter::StopBlocking()
{
	m_pCombatComponent->StopBlocking();
}

void ASwordFightingGameCharacter::Dodge()
{
	// Can't dodge if currently attacking or in the air
	if (!m_pCombatComponent->IsAttacking() && CanJump() && !m_pCombatComponent->IsStaggered())
	{
		// If player is already dodging don't dodge
		if (m_pEvasionComponent->m_bIsDodging)
			return;

		// Manage Stamina
		if (!m_pStatsComponent->DoesMeetStaminaRequirement(10.0f))
		{
			return;
		}
		m_pStatsComponent->UseStamina(10.0f);

		// Call Dodge from evasion component
		m_pEvasionComponent->Dodge(m_bIsSprinting, m_bIsTargetLocked);
	}
}

void ASwordFightingGameCharacter::ToggleTargetLock()
{
	// Toggle the target lock
	m_bIsTargetLocked = !m_bIsTargetLocked;

	// If locking, get the closest enemy position
	if (m_bIsTargetLocked)
	{
		// Try to get the boss
		ABoss* pBoss = Cast<ABoss>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoss::StaticClass()));

		// If the boss is valid then get the position and set lock pos to that position
		if (pBoss != nullptr)
		{
			m_pTargetLockedBoss = pBoss;
		}
	}
}

void ASwordFightingGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASwordFightingGameCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Sprint Bindings
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASwordFightingGameCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASwordFightingGameCharacter::StopSprinting);

	// Block Bindings
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &ASwordFightingGameCharacter::Block);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &ASwordFightingGameCharacter::StopBlocking);

	// Movement Bindings
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ASwordFightingGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ASwordFightingGameCharacter::MoveRight);

	// Attack Bindings
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASwordFightingGameCharacter::HeavyAttack);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Released, this, &ASwordFightingGameCharacter::LightAttack);

	// Dodge Bindings
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ASwordFightingGameCharacter::Dodge);

	// Target Lock Binding
	PlayerInputComponent->BindAction("TargetLock", IE_Pressed, this, &ASwordFightingGameCharacter::ToggleTargetLock);


	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ASwordFightingGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ASwordFightingGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASwordFightingGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASwordFightingGameCharacter::TouchStopped);
}

void ASwordFightingGameCharacter::HandleOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName.ToString() == "Dodge") // Dodge Resetting
	{
		m_pEvasionComponent->m_bIsDodging = false;
	}
	else if (NotifyName.ToString() == "GetUp")
	{
		// Handle Getting up
		GetMesh()->GetAnimInstance()->Montage_Play(m_pCombatComponent->m_pGetUpMontage);
	}
	else if (NotifyName.ToString() == "Death")
	{
		// Handle death here
		if (m_cPlayerDeathScreen != nullptr)
		{
			// Get the death screen
			UUserWidget* pDeathScreen = CreateWidget<UUserWidget>(Cast<APlayerController>(GetController()), m_cPlayerDeathScreen);
			
			// Play the death screen widget animation
			Cast<UGlobalManager>(UGameplayStatics::GetGameInstance(GetWorld()))->PlayWidgetAnimation(pDeathScreen, "Fade");

			// Ragdoll player
			GetMesh()->SetSimulatePhysics(true);

			// Set a timer to wait a few seconds and then restart level
			FTimerHandle pTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(pTimerHandle, [&]()
			{
				UGameplayStatics::OpenLevel(GetWorld(), FName(GetWorld()->GetName()), true);

			}, 2, false);
		}
	}
	else // Attack Combos
	{
		m_iComboIndex--;

		// Handle stamina
		if (!m_pStatsComponent->DoesMeetStaminaRequirement(20.0f))
		{
			m_pCombatComponent->StopAttack(0.35f, "LightAttack");
			return;
		}
		if(m_iComboIndex >= 0)
			m_pStatsComponent->UseStamina(20.0f);

		if (m_iComboIndex < 0 || !m_pStatsComponent->DoesMeetStaminaRequirement(20.0f))
		{
			m_pCombatComponent->StopAttack(0.35f, "LightAttack");
		}
	}
}

void ASwordFightingGameCharacter::HandleOnMontageEnd(UAnimMontage* a_pMontage, bool a_bInterrupted)
{
	// Handle Dodge Montage
	if (a_pMontage->GetName().Contains("Roll"))
	{
		m_pEvasionComponent->m_bIsDodging = false;
		m_pEvasionComponent->m_bCanAttackAfterDodge = true;

		// Play the dodge attack if clicked while dodging
		if (m_pEvasionComponent->m_bAttackingAfterDodge)
		{
			m_pCombatComponent->Attack("DodgeAttack");
			m_pEvasionComponent->m_bCanAttackAfterDodge = false;
			m_pEvasionComponent->m_bAttackingAfterDodge = false;
		}
		else
		{
			// Set timer for dodge attacking
			FTimerHandle pTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(pTimerHandle, [&]()
			{
				// Can no longer do the attack dodge
					m_pEvasionComponent->m_bCanAttackAfterDodge = false;

			}, 1, false);
		}
	}
}

void ASwordFightingGameCharacter::SetupStimulus()
{
	// Create Stimulus Component and register with perception system
	m_pStimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("Stimulus");
	m_pStimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	m_pStimulus->RegisterWithPerceptionSystem();
}

void ASwordFightingGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ASwordFightingGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ASwordFightingGameCharacter::LightAttack()
{
	// Check for attack clicks while dodging
	if (m_pEvasionComponent->m_bIsDodging)
	{
		m_pEvasionComponent->m_bAttackingAfterDodge = true;
	}

	// Light Attack Logic
	m_bIsCharging = false;
	if (!m_pCombatComponent->IsAttacking() && CanJump())
	{
		// If they aren't charged or didn't just heavy attack, play a light attack
		if (!m_bIsCharged)
		{
			// Decide between running attack, and static attack
			if (m_bIsSprinting)
			{
				// Handle stamina
				if (!m_pStatsComponent->DoesMeetStaminaRequirement(20.0f))
				{
					return;
				}
				m_pStatsComponent->UseStamina(20.0f);

				m_pCombatComponent->Attack("SprintAttack");
			}
			else
			{
				// Decide if attack was right after a roll
				if (m_pEvasionComponent->m_bCanAttackAfterDodge)
				{
					// Handle stamina
					if (!m_pStatsComponent->DoesMeetStaminaRequirement(20.0f))
					{
						return;
					}
					m_pStatsComponent->UseStamina(20.0f);

					m_pCombatComponent->Attack("DodgeAttack");
					m_pEvasionComponent->m_bCanAttackAfterDodge = false;
				}
				else
				{
					// Handle stamina
					if (!m_pStatsComponent->DoesMeetStaminaRequirement(20.0f))
					{
						return;
					}
					m_pStatsComponent->UseStamina(20.0f);

					m_pCombatComponent->Attack("LightAttack");
				}
			}
		}
		else // Reset Attack Charge
		{
			m_bIsCharged = false;
		}
	}
	else
	{
		m_iComboIndex = 1;
	}
}

void ASwordFightingGameCharacter::HeavyAttack()
{
	// Ground-Attacks
	if (CanJump())
	{

		if(!m_pEvasionComponent->m_bCanAttackAfterDodge) // Normal Heavy Attack (On the Ground)
		{
			m_bIsCharging = true;

			// Delay check for heavy attack
			FTimerHandle pTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(pTimerHandle, [&]()
				{
					// If still charging, play heavy attack
					if (m_bIsCharging)
					{
						// Handle stamina
						if (!m_pStatsComponent->DoesMeetStaminaRequirement(30.0f))
						{
							return;
						}
						m_pStatsComponent->UseStamina(30.0f);

						m_pCombatComponent->Attack("HeavyAttack");
						m_bIsCharging = false;
						m_bIsCharged = true;
					}

				}, 1, false);
		}
	}
	else // Jump Attacking (In Mid-Air)
	{
		// Handle stamina
		if (!m_pStatsComponent->DoesMeetStaminaRequirement(20.0f))
		{
			return;
		}
		m_pStatsComponent->UseStamina(20.0f);

		m_pCombatComponent->Attack("JumpAttack");
	}
}

void ASwordFightingGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASwordFightingGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASwordFightingGameCharacter::BeginPlay()
{
	// Super call to method
	Super::BeginPlay();

	// Bind Anim Events
	UAnimInstance* pAnimInst = GetMesh()->GetAnimInstance();
	if (pAnimInst != nullptr)
	{
		pAnimInst->OnPlayMontageNotifyBegin.AddDynamic(this, &ASwordFightingGameCharacter::HandleOnMontageNotifyBegin);
		pAnimInst->OnMontageEnded.AddDynamic(this, &ASwordFightingGameCharacter::HandleOnMontageEnd);
	}

	// Attach Player HUD
	if (m_cPlayerHUD != nullptr)
	{
		// Add the HUD to our viewport
		UUserWidget* HUD = CreateWidget<UUserWidget>(Cast<APlayerController>(GetController()), m_cPlayerHUD);
		HUD->AddToViewport(9999);
	}
}

void ASwordFightingGameCharacter::Tick(float a_fDeltaTime)
{
	// Check if player is target locked
	if (m_bIsTargetLocked)
	{
		// Get the lock rotation
		FVector lockLocation = m_pTargetLockedBoss->GetActorLocation() + FVector(0.0f, 0.0f, -100.0f);
		FVector Forward = FVector(lockLocation.X, lockLocation.Y, GetActorLocation().Z) - GetActorLocation();

		//FRotator Rot = UKismetMathLibrary::MakeRotFromXZ(Forward, FVector::UpVector);
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), lockLocation);
		FRotator ActorRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_pTargetLockedBoss->GetActorLocation());

		// Set controller rotation to the new lock rotation
		GetController()->SetControlRotation(Rot);
		SetActorRotation(ActorRot);
	}

	// Handle Sprinting and Stamina
	if (m_bIsSprinting)
	{
		m_pStatsComponent->UseStamina(.15f);
		if (!m_pStatsComponent->DoesMeetStaminaRequirement(1.0f))
		{
			StopSprinting();
		}
	}

}

void ASwordFightingGameCharacter::MoveForward(float Value)
{
	if (!m_pCombatComponent->IsAttacking() && !m_pCombatComponent->m_bIsBlocking && !m_pCombatComponent->IsStaggered() && !m_pCombatComponent->IsDying())
	{
		if ((Controller != nullptr) && (Value != 0.0f))
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}

void ASwordFightingGameCharacter::MoveRight(float Value)
{
	if (!m_pCombatComponent->IsAttacking() && !m_pCombatComponent->m_bIsBlocking && !m_pCombatComponent->IsStaggered() && !m_pCombatComponent->IsDying())
	{
		if ((Controller != nullptr) && (Value != 0.0f))
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
	}
}

void ASwordFightingGameCharacter::Jump()
{
	// Can't jump if currently attacking
	if (!m_pCombatComponent->IsAttacking() && !m_pCombatComponent->IsDying() && !m_pCombatComponent->IsStaggered())
	{
		// Super call to ACharacter Jump
		Super::Jump();

		// Only play animation if player can jump
		if (CanJump())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(m_pJumpMontage);
		}
	}
}

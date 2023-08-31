// Fill out your copyright notice in the Description page of Project Settings.


#include "HurtBox.h"
#include "SwordFightingGameCharacter.h"

// Sets default values
AHurtBox::AHurtBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the capsule component
	m_pCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Collider");
	m_pCapsuleComponent->InitCapsuleSize(20.0f, 20.0f);
	m_pCapsuleComponent->SetupAttachment(RootComponent);

	// Register Overlap Events
	m_pCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AHurtBox::OnCapsuleBeginOverlap);
}

// Called when the game starts or when spawned
void AHurtBox::BeginPlay()
{
	Super::BeginPlay();

	// Set timer for destroying actor
	FTimerHandle pTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(pTimerHandle, [&]()
	{
		// Destroy the box
		Destroy();

	}, m_fLifetime, false);
}

// Called every frame
void AHurtBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHurtBox::OnCapsuleBeginOverlap(UPrimitiveComponent* a_pOverlappedComponent, AActor* a_pOtherActor, UPrimitiveComponent* a_pOtherComp, int32 a_iOtherBodyIndex, bool a_bFromSweep, const FHitResult& a_pSweepResult)
{
	// Check if hurt box is hitting the player
	ASwordFightingGameCharacter* pPlayer = Cast<ASwordFightingGameCharacter>(a_pOtherActor);
	if (pPlayer) // Continue if valid
	{
		// Don't deal damage if player is dodging
		if (!pPlayer->m_bIsDodging)
		{
			// Play hurt animation and deal damage
			if (pPlayer->m_pHurtMontage != nullptr)
			{
				pPlayer->GetMesh()->GetAnimInstance()->Montage_Play(pPlayer->m_pHurtMontage);
				pPlayer->TakeDamage(10.0f);
			}
		}
	}
}


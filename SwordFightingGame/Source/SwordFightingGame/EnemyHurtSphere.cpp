// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHurtSphere.h"

// Sets default values
AEnemyHurtSphere::AEnemyHurtSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the capsule component
	m_pSphereComponent = CreateDefaultSubobject<USphereComponent>("Collider");
	m_pSphereComponent->InitSphereRadius(20.0f);
	m_pSphereComponent->SetupAttachment(RootComponent);

	// Register Overlap Events
	m_pSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyHurtSphere::OnCapsuleBeginOverlap);
}

// Called when the game starts or when spawned
void AEnemyHurtSphere::BeginPlay()
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
void AEnemyHurtSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyHurtSphere::OnCapsuleBeginOverlap(UPrimitiveComponent* a_pOverlappedComponent, AActor* a_pOtherActor, UPrimitiveComponent* a_pOtherComp, int32 a_iOtherBodyIndex, bool a_bFromSweep, const FHitResult& a_pSweepResult)
{

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "MoltenSword.h"

// Sets default values
AMoltenSword::AMoltenSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the mesh
	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>("Sword Mesh");
	m_pMesh->SetupAttachment(RootComponent);

	// Create the hit point tracker
	m_pHitPointTracker = CreateDefaultSubobject<USceneComponent>("Hit Tracker");
	m_pHitPointTracker->SetRelativeLocation(FVector(0.0f, -40.0f, 0.0f));
	m_pHitPointTracker->SetupAttachment(m_pMesh);
}

UStaticMeshComponent* AMoltenSword::GetMesh()
{
	return m_pMesh;
}

// Called when the game starts or when spawned
void AMoltenSword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoltenSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoltenSword.generated.h"

UCLASS()
class SWORDFIGHTINGGAME_API AMoltenSword : public AActor
{
	GENERATED_BODY()
	
	// Sword Mesh
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* m_pMesh;

	// Reference point for generating hurt boxes
	UPROPERTY(VisibleDefaultsOnly, Category = "Tracking")
	USceneComponent* m_pHitPointTracker;

public:	
	// Sets default values for this actor's properties
	AMoltenSword();

	// Getter for mesh
	UStaticMeshComponent* GetMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "EnemyHurtSphere.generated.h"

UCLASS()
class SWORDFIGHTINGGAME_API AEnemyHurtSphere : public AActor
{
	GENERATED_BODY()
	
	// Trigger Capsule Collider
	UPROPERTY(VisibleAnywhere, Category = "Trigger Sphere")
	USphereComponent* m_pSphereComponent;

public:	
	// Sets default values for this actor's properties
	AEnemyHurtSphere();

	// Set the lifetime for the hurt box
	float m_fLifetime = 0.2f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// BeginOverlap Event Handler
	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* a_pOverlappedComponent, AActor* a_pOtherActor, UPrimitiveComponent* a_pOtherComp, int32 a_iOtherBodyIndex, bool a_bFromSweep, const FHitResult& a_pSweepResult);

};

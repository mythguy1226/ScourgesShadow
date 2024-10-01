// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BossTrigger.generated.h"

UCLASS()
class SWORDFIGHTINGGAME_API ABossTrigger : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Trigger Box")
	UBoxComponent* m_pBoxComponent;
	
public:	
	// Sets default values for this actor's properties
	ABossTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* m_pBoss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString m_sName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Code for when something overlaps this component */
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

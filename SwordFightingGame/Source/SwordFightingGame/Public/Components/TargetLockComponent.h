// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetLockComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDFIGHTINGGAME_API UTargetLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetLockComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Toggle target locking
	void ToggleTargetLock();

	// Generate lock query
	void GenerateLockQuery(FVector a_vBeginLoc, FVector a_vForward, float a_fRange, float a_fRadius, bool a_bDebug);

	// Gets the next nearby target
	void GetNextTarget(float a_fDirection);

	// Turn off target lock
	void DisableLock();

	// Actor currently locked in on
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* m_pTargetLockedActor;

	// Flag that indicates lock-state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool m_bIsTargetLocked = false;

	// Fields for lock queries
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fLockQueryRange = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fLockQueryRadius = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_bDebugMode = false;
};

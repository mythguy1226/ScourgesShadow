// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Boss_Controller.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API ABoss_Controller : public AAIController
{
	GENERATED_BODY()
public:
	ABoss_Controller(FObjectInitializer const& a_pObjectInit);

	// Overrides
	void BeginPlay() override;
	void OnPossess(APawn* a_pPawn) override;

	// Method to get black board
	class UBlackboardComponent* GetBlackboard() const;

	// Tree components
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	class UBehaviorTreeComponent* m_pBehaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* m_pBehaviorTree;

private:
	class UBlackboardComponent* m_pBlackboard;

	class UAISenseConfig_Sight* m_pSightConfig;

	UFUNCTION()
	void HandleTargetDetection(AActor* a_pActor, FAIStimulus a_sStimulus);

	void SetupPerception();
};

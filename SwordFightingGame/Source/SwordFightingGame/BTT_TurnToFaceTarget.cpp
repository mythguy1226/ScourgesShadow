// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_TurnToFaceTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "CombatComponent.h"

UBTT_TurnToFaceTarget::UBTT_TurnToFaceTarget(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Turn To Face Target");
}

EBTNodeResult::Type UBTT_TurnToFaceTarget::ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
	// Get Nav System
	UNavigationSystemV1* pNavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	// Get player reference
	APlayerController* pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APawn* pPlayer = pPlayerController->GetPawn();

	// Get combat component from ai controller
	UCombatComponent* pCombatComp = Cast<UCombatComponent>(a_pTreeComp.GetAIOwner()->GetPawn()->GetComponentByClass(UCombatComponent::StaticClass()));

	if (pNavSystem)
	{
		// Return early if attacking
		if(pCombatComp)
			if (pCombatComp->IsAttacking())
				return EBTNodeResult::Succeeded;

		// Turn towards the target
		FVector vLookAtLoc = FVector(pPlayer->GetActorLocation().X, pPlayer->GetActorLocation().Y, a_pTreeComp.GetAIOwner()->GetPawn()->GetActorLocation().Z);
		FRotator pRotation = UKismetMathLibrary::FindLookAtRotation(a_pTreeComp.GetAIOwner()->GetPawn()->GetActorLocation(), vLookAtLoc);
		FRotator pSlerpRot = FQuat::Slerp(a_pTreeComp.GetAIOwner()->GetPawn()->GetActorQuat(), pRotation.Quaternion(), LookRotationRate).Rotator();
		a_pTreeComp.GetAIOwner()->GetPawn()->SetActorRotation(pSlerpRot);
	}

	// Finish execution
	FinishLatentTask(a_pTreeComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

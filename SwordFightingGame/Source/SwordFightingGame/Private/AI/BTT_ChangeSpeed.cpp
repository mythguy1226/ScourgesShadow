// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_ChangeSpeed.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_ChangeSpeed::UBTT_ChangeSpeed(FObjectInitializer const& a_pObjectInit)
{
	NodeName = TEXT("Change Speed");
}

EBTNodeResult::Type UBTT_ChangeSpeed::ExecuteTask(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem)
{
	// Get reference to AI pawn and cast to character
	ACharacter* pAIChar = Cast<ACharacter>(a_pTreeComp.GetAIOwner()->GetPawn());

	// Check if valid and if so,
	// update their movement speed
	if (pAIChar)
	{
		UCharacterMovementComponent* pMoveComp = pAIChar->GetCharacterMovement();
		pMoveComp->MaxWalkSpeed = m_fNewSpeed;
	}

	// Finish execution
	FinishLatentTask(a_pTreeComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTD_DistanceToPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UBTD_DistanceToPlayer::UBTD_DistanceToPlayer()
{
	NodeName = TEXT("Distance Check");
}

bool UBTD_DistanceToPlayer::CalculateRawConditionValue(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem) const
{
	// Super call to decorator
	bool bSuccess = Super::CalculateRawConditionValue(a_pTreeComp, a_pNodeMem);
	if (!bSuccess) return false;

	// Get distance between pawn and player
	float fDistance = a_pTreeComp.GetAIOwner()->GetPawn()->GetDistanceTo(Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)));

	// Return if distance is within configured range
	return fDistance >= m_fNearDistance && fDistance <= m_fFarDistance;
}

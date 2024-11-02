// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RandomChance.h"

URandomChance::URandomChance()
{
	NodeName = TEXT("Random Chance");
}

bool URandomChance::CalculateRawConditionValue(UBehaviorTreeComponent& a_pTreeComp, uint8* a_pNodeMem) const
{
	// Super call to decorator
	bool bSuccess = Super::CalculateRawConditionValue(a_pTreeComp, a_pNodeMem);
	if (!bSuccess) return false;

	// Get random float
	float fRandomChoice = FMath::RandRange(0.0f, 100.0f);

	// Return whether the choice succeeded
	return fRandomChoice <= m_fChance;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_DefeatEnemy.h"
#include "Enemy.h"

void UAN_DefeatEnemy::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// Get owner of the mesh component
	AActor* pOwner = MeshComp->GetOwner();

	// If cast is successful, reset the boss
	AEnemy* pEnemy = Cast<AEnemy>(pOwner);
	if (pEnemy)
		pEnemy->Reset();
}

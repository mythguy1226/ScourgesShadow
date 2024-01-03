// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_DefeatBoss.h"
#include "Boss.h"

void UAN_DefeatBoss::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// Get owner of the mesh component
	AActor* pOwner = MeshComp->GetOwner();

	// If cast is successful, reset the boss
	ABoss* pBoss = Cast<ABoss>(pOwner);
	if (pBoss)
		pBoss->Reset();
}

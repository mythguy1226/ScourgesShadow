// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_DefeatEnemy.h"
#include "TargetLockComponent.h"
#include "Enemy.h"

void UAN_DefeatEnemy::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// Get owner of the mesh component
	AActor* pOwner = MeshComp->GetOwner();

	// Get player's target lock component and disable if 
	// not targeting a new enemy
	ACharacter* pPlayer = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(pOwner->GetWorld(), 0));
	UTargetLockComponent* pTargetComp = Cast<UTargetLockComponent>(pPlayer->GetComponentByClass(UTargetLockComponent::StaticClass()));
	if (pTargetComp->m_pTargetLockedActor)
		if (pTargetComp->m_pTargetLockedActor == pOwner)
			pTargetComp->DisableLock();

	// If cast is successful, reset the boss
	AEnemy* pEnemy = Cast<AEnemy>(pOwner);
	if (pEnemy)
		pEnemy->Reset();
}

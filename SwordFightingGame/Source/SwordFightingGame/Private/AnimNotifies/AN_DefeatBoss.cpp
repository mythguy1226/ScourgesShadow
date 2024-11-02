// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_DefeatBoss.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TargetLockComponent.h"
#include "AI/BossManager.h"
#include "AI/Boss.h"

void UAN_DefeatBoss::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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

	// Get reference to boss manager
	ABossManager* pBossMngr = Cast<ABossManager>(UGameplayStatics::GetActorOfClass(MeshComp->GetOwner()->GetWorld(), ABossManager::StaticClass()));

	// If not already in a fight, start the boss fight with the selected boss
	if (pBossMngr->m_bFightActive)
	{
		pBossMngr->EndBossFight();
	}

	// Destroy actor
	pOwner->K2_DestroyActor();
}

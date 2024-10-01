// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_HealPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "CombatComponent.h"

void UAN_HealPlayer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// Get reference to player
	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(MeshComp->GetOwner()->GetWorld(), 0);
	if (pPlayer)
	{
		// Get reference to combat component
		UCombatComponent* pCombatComp = Cast<UCombatComponent>(pPlayer->GetComponentByClass(UCombatComponent::StaticClass()));
		
		// Call heal method
		if (pCombatComp)
			pCombatComp->Heal();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_PlayerDeath.h"
#include "Kismet/GameplayStatics.h"

void UAN_PlayerDeath::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// Restart the level
	UGameplayStatics::OpenLevel(MeshComp->GetOwner()->GetWorld(), FName(MeshComp->GetOwner()->GetWorld()->GetName()), true);
}

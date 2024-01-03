// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_PlayerDeath.h"
#include "Kismet/GameplayStatics.h"

void UAN_PlayerDeath::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// Set a timer to wait a few seconds and then restart level
	FTimerHandle pTimerHandle;
	MeshComp->GetOwner()->GetWorld()->GetTimerManager().SetTimer(pTimerHandle, [&]()
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(MeshComp->GetOwner()->GetWorld()->GetName()), true);

	}, 2, false);
}

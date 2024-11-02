// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_PlayWidgetAnim.h"
#include "General/GlobalManager.h"
#include "General/UIManager.h"
#include "Kismet/GameplayStatics.h"

void UAN_PlayWidgetAnim::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// Get the widget
	UUserWidget* pWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(MeshComp->GetOwner()->GetWorld(), 0), m_cWidgetClass);

	// Get global manager
	UGlobalManager* pMngr = Cast<UGlobalManager>(UGameplayStatics::GetGameInstance(MeshComp->GetOwner()->GetWorld()));
	if (pMngr)
	{
		// Get the UI Manager
		AUIManager* pUIMngr = Cast<AUIManager>(pMngr->GetService<AUIManager>());

		// Play the widget animation
		if (pUIMngr)
			pUIMngr->PlayWidgetAnimation(pWidget, m_sAnimationName, m_bReversed);
	}
}

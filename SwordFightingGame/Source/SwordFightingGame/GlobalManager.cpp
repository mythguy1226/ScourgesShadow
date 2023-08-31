// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UGlobalManager::PlayWidgetAnimation(UUserWidget* a_pWidget, FString a_sName, bool a_bReverse)
{
	// Return if the widget is null
	if (a_pWidget == nullptr)
	{
		return;
	}

	// Get all widget animations from death screen
	UProperty* Prop = a_pWidget->GetClass()->PropertyLink;

	// Variable to store widget anim
	UWidgetAnimation* pAnim = nullptr;

	// Run through all properties of this class to find any widget animations
	while (Prop != nullptr)
	{
		// Only interested in object properties
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			// Only want the properties that are widget animations
			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(a_pWidget);

				UWidgetAnimation* WidgetAnim = Cast<UWidgetAnimation>(Obj);

				if (WidgetAnim != nullptr && WidgetAnim->MovieScene != nullptr)
				{
					FName AnimName = WidgetAnim->MovieScene->GetFName();
					if (AnimName.ToString() == a_sName)
					{
						pAnim = WidgetAnim;
						break;
					}
				}
			}
		}

		Prop = Prop->PropertyLinkNext;
	}

	// Play the widget animation if valid
	if (pAnim != nullptr)
	{
		// Play the animation backwards
		if (a_bReverse)
		{
			a_pWidget->AddToViewport(0);
			a_pWidget->PlayAnimation(pAnim, pAnim->GetEndTime(), 1, EUMGSequencePlayMode::Reverse);
		}
		else // Play the animation normally
		{
			a_pWidget->AddToViewport(0);
			a_pWidget->PlayAnimation(pAnim);
		}
	}
}

void UGlobalManager::PlayDelayedWidgetAnimation(UUserWidget* a_pWidget, FString a_sName, bool a_bReverse, float a_fDelay)
{
	// Set a timer for playing the widget animation
	FTimerHandle pTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(pTimerHandle, [&]()
	{
		// Play the widget animation
		PlayWidgetAnimation(a_pWidget, a_sName, a_bReverse);

	}, a_fDelay, false);
}

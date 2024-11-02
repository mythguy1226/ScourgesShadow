// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "General/GlobalService.h"
#include "GlobalManager.generated.h"

/**
 * 
 */
UCLASS()
class SWORDFIGHTINGGAME_API UGlobalManager : public UGameInstance
{
	GENERATED_BODY()
private:
	// Stores all services that can be referenced later
	TArray<AActor*> m_aServices;

public:
	// Overridden methof for game instance initialization
	virtual void Init() override;

	// Method used for loading services into array
	void LoadServices();

	// Templated method for getting services of class type
	template<class T>
	AActor* GetService() 
	{
		// Iterate through services until desired class is found
		for (AActor* pService : m_aServices)
		{
			// Check if correct class
			if (Cast<T>(pService))
				return pService;
		}

		return nullptr;
	}
};

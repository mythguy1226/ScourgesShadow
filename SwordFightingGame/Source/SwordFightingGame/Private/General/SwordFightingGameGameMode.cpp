// Copyright Epic Games, Inc. All Rights Reserved.

#include "General/SwordFightingGameGameMode.h"
#include "Player/SwordFightingGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASwordFightingGameGameMode::ASwordFightingGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

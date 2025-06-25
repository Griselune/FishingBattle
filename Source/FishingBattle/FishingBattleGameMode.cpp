// Copyright Epic Games, Inc. All Rights Reserved.

#include "FishingBattleGameMode.h"
#include "FishingBattleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFishingBattleGameMode::AFishingBattleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

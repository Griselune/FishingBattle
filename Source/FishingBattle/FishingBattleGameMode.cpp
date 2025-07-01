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

void AFishingBattleGameMode::RespawnPlayer(AController* Controller)
{
    if (!Controller) return;

    AActor* StartSpot = FindPlayerStart(Controller);
    if (!StartSpot) return;

    FVector Location = StartSpot->GetActorLocation();
    FRotator Rotation = StartSpot->GetActorRotation();

    APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, Location, Rotation);
    if (NewPawn)
    {
        Controller->Possess(NewPawn);
    }
}

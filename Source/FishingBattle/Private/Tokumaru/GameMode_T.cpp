// Fill out your copyright notice in the Description page of Project Settings.


#include "Tokumaru/GameMode_T.h"
#include "UObject/ConstructorHelpers.h"

AGameMode_T::AGameMode_T()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}

void AGameMode_T::RespawnPlayerT(AController* Controller)
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

TSubclassOf<AActor> AGameMode_T::GetWeaponClass(FName WeaponID) const
{
    if (weaponMap.Contains(WeaponID))
    {
        return weaponMap[WeaponID];
    }
    return nullptr;
}

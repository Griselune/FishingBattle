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
    // 2025.07.25 ウー start
    bUseSeamlessTravel = true;
    // 2025.07.25 ウー end
}

void AGameMode_T::RespawnPlayerT(AController* Controller)
{
    if (!Controller) return;

    AActor* StartSpot = FindPlayerStart(Controller);
    if (!StartSpot) return;

    //FVector Location = StartSpot->GetActorLocation();
    //FRotator Rotation = StartSpot->GetActorRotation();


    //FRandomStream Stream;
    //Stream.GenerateNewSeed();  // 毎回違う乱数列
    //float randomX = Stream.RandRange(-50.0f, 50.0f);
    //float randomY = Stream.RandRange(-50.0f, 50.0f);

    //Location.X += randomX;
    //Location.Y += randomY;
    FVector Location;
    FRotator Rotation;
    bool bHit = false;
    do {
        Location = StartSpot->GetActorLocation();
        Rotation = StartSpot->GetActorRotation();


        FRandomStream Stream;
        Stream.GenerateNewSeed();  // 毎回違う乱数列
        float randomX = Stream.RandRange(-200.0f, 200.0f);
        float randomY = Stream.RandRange(-200.0f, 200.0f);

        Location.X += randomX;
        Location.Y += randomY;

        FVector Start = Location;
        FVector End = Start + FVector(0, 0, 100.0f);

        FHitResult HitResult;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Controller->GetPawn()); // 自分自身を無視

        bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
    } while (bHit);


    UE_LOG(LogTemp, Log, TEXT("StartSpot Location: X=%f, Y=%f, Z=%f"), Location.X, Location.Y, Location.Z);

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

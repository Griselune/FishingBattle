// Copyright Epic Games, Inc. All Rights Reserved.

#include "FishingBattleGameMode.h"
#include "FishingBattleCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Public/WuBranch/Actor/FishingGround.h"
#include "Public/WuBranch/Actor/FishingGroundSpawnArea.h"
#include <Kismet/GameplayStatics.h>

AFishingBattleGameMode::AFishingBattleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

// 2025.07.06 ウー start
void AFishingBattleGameMode::StartPlay()
{
    Super::StartPlay();

    /*AActor* FindActor = UGameplayStatics::GetActorOfClass(GetWorld(), AFishingGroundSpawnArea::StaticClass());
    FishingGroundSpawnArea = Cast<AFishingGroundSpawnArea>(FindActor);
    SpawnFishingGroundRandom();*/
}
// 2025.07.06 ウー end

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
        NewPawn->SetOwner(Controller);
        Controller->Possess(NewPawn);
    }
    //UE_LOG(LogTemp, Warning, TEXT("[Respawn] Controller: %s (HasAuthority: %d, IsPlayerController: %d)"),
    //    *GetNameSafe(Controller),
    //    Controller->HasAuthority() ? 1 : 0,
    //    Cast<APlayerController>(Controller) != nullptr ? 1 : 0
    //);
}

void AFishingBattleGameMode::SpawnFishingGroundRandom()  
{  
    // 設定してない場合  
    if (!FishingGroundSample || !FishingGroundSpawnArea)  
    {  
        UE_LOG(LogTemp, Error, TEXT("Didn't set Fishing Ground or Spawn Area"));  
        return;  
    }  

    for (int index = 0; index < FishingGroundNums; index++)  
    {  
        float Longest = FishingGroundSpawnArea->GetFarthestRadius();  
        float Shortest = FishingGroundSpawnArea->GetIslandRadius();  
        float Distance = FMath::RandRange(Shortest, Longest);  

        FVector Dir = FMath::VRand();  
        Dir.Z = 0.0f;  
        FVector Center = FishingGroundSpawnArea->GetActorLocation();  
        FVector Location = Center + Dir * Distance;  
        FString txt = FString::Printf(TEXT("Dir: %s"), *Location.ToString());  
        

        AFishingGround* Ground = GetWorld()->SpawnActor<AFishingGround>(FishingGroundSample, Location, FRotator());
        if (Ground)
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Spawn Succ"));
        Grounds.Add(Ground);
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, txt);
    }
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Count: %d"), Grounds.Num()));
}
// 2025.07.06 ウー end

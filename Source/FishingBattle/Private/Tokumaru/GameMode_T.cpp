// Fill out your copyright notice in the Description page of Project Settings.


#include "Tokumaru/GameMode_T.h"
#include "UObject/ConstructorHelpers.h"
#include <WuBranch/GameState/DeathMatchGameState.h>
#include <Tokumaru/PlayerState_T.h>
#include <PrinzBranch/LANGameInstance.h>
#include <WuBranch/Struct/PlayerRecord.h>
#include "WuBranch/Library/MyGameplaySystemLibrary.h"
#include "FishingBattle/FishingBattleCharacter.h"

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

    FVector Location;
    FRotator Rotation;
    bool bHit = false;
    do {
        Location = StartSpot->GetActorLocation();
        Rotation = StartSpot->GetActorRotation();


        FRandomStream Stream;
        Stream.GenerateNewSeed(); //種値更新
        float randomX = Stream.RandRange(-800.0f, 800.0f);
        float randomY = Stream.RandRange(-800.0f, 800.0f);

        Location.X += randomX;
        Location.Y += randomY;

        FVector Start = Location;
        FVector End = Start + FVector(0, 0, 500.0f);

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
        //プリンス START 2025/10/25 test
 //       AFishingBattleCharacter* NewChar = Cast<AFishingBattleCharacter>(Controller);
 //       NewChar->UpdateNameWidget();
        //プリンス END 2025/10/25 test
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

// 2025.07.26 ウー start
void AGameMode_T::StartPlay()
{
    Super::StartPlay();

    if (ADeathMatchGameState* GS = GetGameState<ADeathMatchGameState>())
    {
        GS->Server_OnGameStateChanged.AddDynamic(this, &AGameMode_T::OnStateChanged);
    }

    // 2025.10.22 ウー start
    CurrentWinnerID = -1;
	// 2025.10.22 ウー end
}

void AGameMode_T::OnStateChanged(EGameStateList State)
{
    // サーバーのみ実行
    if (!HasAuthority())
        return;

    if (State == EGameStateList::Finished)
    {
        AddPlayerDatasToRecord();
    }
}

void AGameMode_T::AddPlayerDatasToRecord()
{
    ULANGameInstance* GameInstance = GetGameInstance<ULANGameInstance>();
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Did not found GameInstance"));
        return;
    }

    TArray<TObjectPtr<APlayerState>> PlayerStates = GetGameState<AGameState>()->PlayerArray;
    for (APlayerState* PlayerState : PlayerStates)
    {
        if (APlayerState_T* PS = Cast<APlayerState_T>(PlayerState))
        {
            GameInstance->AddRecord(MakeRecord(PS));
        }
    }
}

FPlayerRecord& AGameMode_T::MakeRecord(APlayerState_T* Player)
{
    FPlayerRecord* Record = new FPlayerRecord();
    Record->ID = Player->GetPlayerId();
    Record->DeathCount = Player->DeadCounter;
    Record->TotalDamage = 0.0f;
    Record->Name = Player->GetName();
	Record->Point = Player->GetPoint();
    return *Record;
}

// 2025.07.26 ウー end

// 2025.10.22 ウー start
#pragma region 勝者の更新

void AGameMode_T::UpdateWinner()
{
    // 新しい情報をゲット
    TArray<FPlayerRecord> Records;
    TArray<TObjectPtr<APlayerState>> PlayerStates = GetGameState<AGameState>()->PlayerArray;
    for (APlayerState* PlayerState : PlayerStates)
    {
        if (APlayerState_T* PS = Cast<APlayerState_T>(PlayerState))
        {
            Records.Add(MakeRecord(PS));
        }
    }
    // 新しい勝者を見つける
    FPlayerRecord* NewWinner = UMyGameplaySystemLibrary::FindWinner(Records);
	// 勝者を更新する
    if(CurrentWinnerID == -1 || (NewWinner && CurrentWinnerID != NewWinner->ID))
    {
        UpdateCrown(NewWinner);
        CurrentWinnerID = NewWinner->ID;
	}
}

void AGameMode_T::UpdateCrown(const FPlayerRecord* NewWinner)
{
    AFishingBattleCharacter* Winner;
	// 前回の王冠を外す
    if (CurrentWinnerID != -1)
    {
        // 今王冠持っている人
        Winner = FindPlayer(CurrentWinnerID);
        if (Winner)
        {
            UE_LOG(LogTemp, Warning, TEXT("Pre Winner: %s"), *Winner->GetActorNameOrLabel());
            Winner->HideCrown();
        }
    }
	// 新しい王冠をつける
    Winner = FindPlayer(NewWinner->ID);
    if (Winner)
    {
        UE_LOG(LogTemp, Warning, TEXT("Current Winner: %s"), *Winner->GetActorNameOrLabel());
        Winner->ShowCrown();
    }
}

AFishingBattleCharacter* AGameMode_T::FindPlayer(const int32& TargetID)
{
    TArray<TObjectPtr<APlayerState>> PlayerStates = GetGameState<AGameState>()->PlayerArray;
    for (const APlayerState* PlayerState : PlayerStates)
    {
        if (PlayerState->GetPlayerId() == TargetID)
        {
            return PlayerState->GetPawn<AFishingBattleCharacter>();
        }
    }
    return nullptr;
}
#pragma endregion
// 2025.10.22 ウー end
// Fill out your copyright notice in the Description page of Project Settings.

#include "PrinzBranch/LANGameInstance.h"
//#include "OnlineSessionSettings.h"
//#include "OnlineSubsystem.h"
//#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include <WuBranch/Struct/PlayerRecord.h>


//void ULANGameInstance::OnRep_PlayerName()
//{
//    UE_LOG(LogTemp, Log, TEXT("Player name updated: %s"), *GIPlayerName);
//}


// 2025.07.26 ウー start
TArray<FPlayerRecord> ULANGameInstance::GetRecords() const
{
    return GameRecords;
}

void ULANGameInstance::ClearRecords()
{
    GameRecords.Empty();
}

void ULANGameInstance::AddRecord(const FPlayerRecord& Record)
{
    // 直接に追加
    GameRecords.Add(Record);
}
// 2025.07.26 ウー end

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <WuBranch/Struct/PlayerRecord.h>
#include "LANGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API ULANGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    FString GISessionName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    int32 GISessionPlayerLimit = 1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    int32 GISessionCurrentPlayers = 1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    int32 GISessionTimeLimit = 3;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    bool GIisDeathMatch = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    bool GIisBattleRoyale = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString GIPlayerName = "";  //BluePrintで名前を保存する

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    TMap<APlayerState*, FString> GIPlayerList; //Serverが持っているプレヤーリストのコピー


    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    bool GIisReady = false;


public:
    //UFUNCTION(BlueprintCallable)
    //void OnRep_PlayerName();

    UFUNCTION(BlueprintCallable)
    void SetPlayerName(FString NewName);


    // 2025.07.26 ウー start

    /// <summary>
    /// 記録をゲット
    /// </summary>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable)
    TArray<FPlayerRecord> GetRecords() const;

    /// <summary>
    /// 記録をクリアする
    /// </summary>
    void ClearRecords();

    /// <summary>
    /// 記録する
    /// </summary>
    /// <param name="Record"></param>
    void AddRecord(const FPlayerRecord& Record);
    // 2025.07.26 ウー end

private:
    //void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

    //TSharedPtr<class FOnlineSessionSettings> SessionSettings;


    // 2025.07.26 ウー start
    /// <summary>
    /// 記録
    /// </summary>
    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TArray<FPlayerRecord> GameRecords;
    // 2025.07.26 ウー end
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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
    FString HostSessionName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    FString HostSessionPassword;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    int32 HostSessionPlayerLimit = 1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    int32 HostSessionTimeLimit = 3;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    bool isDeathMatch = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    bool isBattleRoyale = true;

    UFUNCTION(BlueprintCallable, Category = "LAN Session")
    void CreateLANSession(const FString& SessionName, const FString& Password, const int32& PlayerLimit, const int32& TimeLimit); //added playerlimit and timelimit

private:
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

    TSharedPtr<class FOnlineSessionSettings> SessionSettings;
};

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
    int32 HostSessionPlayerLimit;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    int32 HostSessionTimeLimit;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    bool isDeathMatch;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    bool isBattleRoyale;

    UFUNCTION(BlueprintCallable, Category = "LAN Session")
    void CreateLANSession(const FString& SessionName, const FString& Password);

private:
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

    TSharedPtr<class FOnlineSessionSettings> SessionSettings;
};

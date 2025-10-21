// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchMakingWidget.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API UMatchMakingWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
    class UButton* BTN_ReadyHost;

    UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
    class UButton* BTN_PlayNow;

    UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
    class UTextBlock* TXT_SessionPlayerLimit;

    UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
    class UTextBlock* TXT_SessionPlayerMax;

    UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
    class UTextBlock* TXT_SessionName;

    UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
    class UTextBlock* TXT_SessionGameMode;

    UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
    class UTextBlock* TXT_SessionTimeLimit;




    //"Ready"ボタンの色を管理する
    UFUNCTION(BlueprintCallable)
    void SetButtonReadyColor(bool bIsReady);

    //"Play Now"ボタンの色を管理する
    UFUNCTION(BlueprintCallable)
    void SetButtonPlayNowColor(bool bIsReady);

    //TXT_SessionPlayerLimitの表示。現在の準備済みプレヤー表示を管理する
    UFUNCTION(BlueprintCallable)
    void SetTextReadyPlayers(const int32& readyplayers, const int32& currentplayers);

    //TXT_SessionNameの表示
    UFUNCTION(BlueprintCallable)
    void SetTextSessionName(const FString& SessionName_);

    //TXT_SessionPlayerMaxの表示
    UFUNCTION(BlueprintCallable)
    void SetTextPlayerMax(const FString& SessionPlayerLimit_);

    //TXT_SessionGameModeの表示
    UFUNCTION(BlueprintCallable)
    void SetTextSessionGameMode(const FString& SessionGameMode_);

    //TXT_SessionTimeLimitの表示
    UFUNCTION(BlueprintCallable)
    void SetTextSessionTimeLimit(const FString& SessionTimeLimit_);
};

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

    //"Ready"ボタンの色を管理する
    UFUNCTION(BlueprintCallable)
    void SetButtonReadyColor(bool bIsReady);

    //"Play Now"ボタンの色を管理する
    UFUNCTION(BlueprintCallable)
    void SetButtonPlayNowColor(bool bIsReady);

    //現在の準備済みプレヤー表示を管理する
    UFUNCTION(BlueprintCallable)
    void SetTextReadyPlayers(const int32& readyplayers, const int32& currentplayers);

};

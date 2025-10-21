// Fill out your copyright notice in the Description page of Project Settings.


#include "PrinzBranch/MatchMakingWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMatchMakingWidget::SetButtonReadyColor(bool bIsReady)
{
    if (!BTN_ReadyHost) return;

    FLinearColor Color = bIsReady ? FColor::FromHex("74FF8DFF") : FColor::FromHex("FFB19AFF");
    // Changes the button’s background tint (UMG)
    BTN_ReadyHost->WidgetStyle.Normal.TintColor = FSlateColor(Color);
    BTN_ReadyHost->WidgetStyle.Hovered.TintColor = FSlateColor(Color);
    BTN_ReadyHost->WidgetStyle.Pressed.TintColor = FSlateColor(Color);
}

void UMatchMakingWidget::SetButtonPlayNowColor(bool bIsReady)
{
    if (!BTN_PlayNow) return;

    //色　：　　　　　　　　　　　　　　　　　　　　　　グリーン　　　/　　　　　　　　　赤　
    FLinearColor Color = bIsReady ? FColor::FromHex("74FF8DFF") : FColor::FromHex("FFB19AFF");
     
    //Changes the button’s background tint (UMG)
    BTN_PlayNow->WidgetStyle.Normal.TintColor = FSlateColor(Color);
    BTN_PlayNow->WidgetStyle.Hovered.TintColor = FSlateColor(Color);
    BTN_PlayNow->WidgetStyle.Pressed.TintColor = FSlateColor(Color);

    //全員が準備OKだったら、ゲーム開始ボタンを押すことが出来る
    BTN_PlayNow->SetIsEnabled(bIsReady);
}

void UMatchMakingWidget::SetTextReadyPlayers(const int32& readyplayers, const int32& currentplayers)
{
    if (!TXT_SessionPlayerLimit) return;

    FString NewText = FString::FromInt(readyplayers) + "/" + FString::FromInt(currentplayers);

    TXT_SessionPlayerLimit->SetText(FText::FromString(NewText));
}

void UMatchMakingWidget::SetTextSessionName(const FString& SessionName_)
{
    if (!TXT_SessionName) return;

    TXT_SessionName->SetText(FText::FromString(SessionName_));
}

void UMatchMakingWidget::SetTextPlayerMax(const FString& SessionPlayerLimit_)
{
    if (!TXT_SessionPlayerMax) return;

    TXT_SessionPlayerMax->SetText(FText::FromString(SessionPlayerLimit_));
}

void UMatchMakingWidget::SetTextSessionGameMode(const FString& SessionGameMode_)
{
    if (!TXT_SessionGameMode) return;

    TXT_SessionGameMode->SetText(FText::FromString(SessionGameMode_));
}

void UMatchMakingWidget::SetTextSessionTimeLimit(const FString& SessionTimeLimit_)
{
    if (!TXT_SessionTimeLimit) return;

    TXT_SessionTimeLimit->SetText(FText::FromString(SessionTimeLimit_));
}





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

    FLinearColor Color = bIsReady ? FColor::FromHex("74FF8DFF") : FColor::FromHex("FFB19AFF");
     
    // Changes the button’s background tint (UMG)
    BTN_PlayNow->WidgetStyle.Normal.TintColor = FSlateColor(Color);
    BTN_PlayNow->WidgetStyle.Hovered.TintColor = FSlateColor(Color);
    BTN_PlayNow->WidgetStyle.Pressed.TintColor = FSlateColor(Color);
}

void UMatchMakingWidget::SetTextReadyPlayers(const int32& readyplayers, const int32& currentplayers)
{
    if (!TXT_SessionPlayerLimit) return;

    FString NewText = FString::FromInt(readyplayers) + "/" + FString::FromInt(currentplayers);

    TXT_SessionPlayerLimit->SetText(FText::FromString(NewText));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/PlayerController/ResultPlayerController.h"
#include <WuBranch/UI/BaseHUD.h>
#include "Blueprint/UserWidget.h"
#include <WuBranch/Interface/NameUI.h>
#include <WuBranch/GameMode/ResultMode.h>

AResultPlayerController::AResultPlayerController()
{
	
}

void AResultPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AddWidgetToView();
	SetWinnerName();
}

void AResultPlayerController::ShowWinnerName_Implementation()
{
	if (HUDWidget && HUDWidget->Implements<UNameUI>())
	{
		INameUI::Execute_ShowName(HUDWidget);
	}
}

void AResultPlayerController::AddWidgetToView_Implementation()
{
	if (HUDUIClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDUIClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			HUDWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HUDWidget is null!"));
		}
	}
}

void AResultPlayerController::SetWinnerName_Implementation()
{
	// 勝者の名前をリクエスト
	RequestWinnerName();
}

void AResultPlayerController::RequestWinnerName_Implementation()
{
	AResultMode* ResultMode = GetWorld()->GetAuthGameMode<AResultMode>();
	if (ResultMode)
	{
		ReceiveWinnerName(ResultMode->GetWinnerName());
	}
}

void AResultPlayerController::ReceiveWinnerName_Implementation(const FString& WinnerName)
{
	if (HUDWidget && HUDWidget->Implements<UNameUI>())
	{
		INameUI::Execute_SetName(HUDWidget, WinnerName);
	}
}
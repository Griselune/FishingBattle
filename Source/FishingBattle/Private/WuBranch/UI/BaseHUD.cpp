// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/UI/BaseHUD.h"
#include "Blueprint/UserWidget.h"

ABaseHUD::ABaseHUD()
{
}

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	AddWidgetToView();
}

void ABaseHUD::AddWidgetToView()
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

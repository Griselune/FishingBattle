// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/PlayerController/FisherController.h"
#include "WuBranch/UI/BaseHUD.h"
#include <WuBranch/Interface/HPUpdaterInterface.h>
#include <WuBranch/Interface/HUDInterface.h>
#include "Blueprint/UserWidget.h"
#include <Kismet/KismetSystemLibrary.h>
#include <FishingBattle/FishingBattleCharacter.h>

AFisherController::AFisherController()
{
	InventoryUI = nullptr;
	HPUI = nullptr;
}

void AFisherController::BeginPlay()
{
	MyHUD = GetHUD<ABaseHUD>();

}

void AFisherController::OnPossess(APawn* aPawn)
{
	// サーバーだけが呼べる
	Super::OnPossess(aPawn);

	// 自分のコントローラーなら
	if (IsLocalController())
	{
		CreateUI();
	}
}

void AFisherController::ShowFinishUI(bool IsWin)
{
	if (IHUDInterface* HUDInterface = Cast<IHUDInterface>(MyHUD))
	{
		HUDInterface->ShowFinishUI(IsWin);
	}
}

void AFisherController::UpdateHP(float Max, float Current)
{
	if (!IsLocalController())
		return;

	if (UKismetSystemLibrary::DoesImplementInterface(HPUI, UHPUpdaterInterface::StaticClass()))
	{
		IHPUpdaterInterface::Execute_UpdateHP(HPUI, Max, Current);
	}
}

void AFisherController::OnRep_Pawn()
{
	// クライアントだけが呼べる
	// 自分のコントローラーなら
	if (IsLocalController())
	{
		CreateUI();
	}
}

void AFisherController::CreateUI()
{
	// インベントリのUI
	if (!InventoryUI && InventoryUIClass)
	{
		InventoryUI = CreateWidget<UUserWidget>(GetWorld(), InventoryUIClass);
		InventoryUI->AddToViewport();
	}

	if (!HPUI)
	{
		if (HPUIClass)
		{
			HPUI = CreateWidget<UUserWidget>(GetWorld(), HPUIClass);
			HPUI->AddToViewport();
		}
	}
	else
	{
		if (AFishingBattleCharacter* MyChara = GetPawn<AFishingBattleCharacter>())
		{
			UpdateHP(MyChara->GetMaxHealth(), MyChara->GetMaxHealth());
		}
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/PlayerController/FisherController.h"
#include "WuBranch/UI/BaseHUD.h"
#include <WuBranch/Interface/HPUpdaterInterface.h>
#include <WuBranch/Interface/HUDInterface.h>
#include "Blueprint/UserWidget.h"
#include <Kismet/KismetSystemLibrary.h>

//10月8日　滝本海大　開始
#include "TakimotoBranch/CPP_FishingSkillCheck.h"
#include <TakimotoBranch/Interface/UIActivity.h>
//10月8日　滝本海大　終了

#include <FishingBattle/FishingBattleCharacter.h>

AFisherController::AFisherController()
{
	InventoryUI = nullptr;
	HPUI = nullptr;

	//10月8日　滝本海大　開始
	FishingGaugeUI = nullptr;
	//10月8日　滝本海大　終了
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

	//10月8日　滝本海大　開始
	//釣るときのゲージUI
	if (!FishingGaugeUI) {
		if (FishingGaugeUIClass) {
			FishingGaugeUI = CreateWidget<UUserWidget>(GetWorld(), FishingGaugeUIClass);
			UE_LOG(LogTemp, Display, TEXT("FishingGaugeUI set"));
		}
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("FishingGaugeUI already set"));
	}
	//10月8日　滝本海大　終了
}

//10月8日　滝本海大　開始
void AFisherController::ShowFishingGauge()
{
	if (HasAuthority()) { UE_LOG(LogTemp, Display, TEXT("Server ShowFishingGauge()")); }
	else UE_LOG(LogTemp, Display, TEXT("Client ShowFishingGauge()"));

	if (IsValid(FishingGaugeUI)) {
		if (FishingGaugeUI->Implements<UUIActivity>())
		{
			IUIActivity::Execute_ShowUI(FishingGaugeUI);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("FishingGaugeUI don't set")); 
	}
}

void AFisherController::StopGauge()
{
	/*if (UCPP_FishingSkillCheck* FishingSkillCheck = Cast<UCPP_FishingSkillCheck>(MyHUD))
	{
		FishingSkillCheck->Stop();
	}*/
}
//10月8日　滝本海大　終了
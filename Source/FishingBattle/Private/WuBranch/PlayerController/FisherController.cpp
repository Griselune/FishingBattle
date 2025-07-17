// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/PlayerController/FisherController.h"
#include "WuBranch/UI/BaseHUD.h"
#include <WuBranch/Interface/HPUpdaterInterface.h>
#include <WuBranch/Interface/HUDInterface.h>

AFisherController::AFisherController()
{
}

void AFisherController::BeginPlay()
{
	MyHUD = GetHUD<ABaseHUD>();

}

void AFisherController::ShowFinishUI(bool IsWin)
{
	if (IHUDInterface* HUDInterface = Cast<IHUDInterface>(MyHUD))
	{
		HUDInterface->ShowFinishUI(IsWin);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FisherController.generated.h"

class ABaseHUD;

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API AFisherController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFisherController();

protected:

	virtual void BeginPlay() override;
	
public:

	/// <summary>
	/// ゲーム終了時のUIを表示
	/// </summary>
	/// <param name="IsWin">true: 勝利, false: 負け</param>
	void ShowFinishUI(bool IsWin);

private:

	ABaseHUD* MyHUD;

};

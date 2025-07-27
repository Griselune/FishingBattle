// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FisherController.generated.h"

class ABaseHUD;
class UUserWidget;

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
	
	virtual void OnPossess(APawn* aPawn) override;

public:

	/// <summary>
	/// ゲーム終了時のUIを表示
	/// </summary>
	/// <param name="IsWin">true: 勝利, false: 負け</param>
	void ShowFinishUI(bool IsWin);

	/// <summary>
	/// HPバーを更新
	/// </summary>
	/// <param name="MaxHP">最大HP</param>
	/// <param name="NewHP">新しいHP</param>
	void UpdateHP(float Max, float Current);

	void OnRep_Pawn() override;


private:

	/// <summary>
	/// UIを生成
	/// </summary>
	void CreateUI();


	ABaseHUD* MyHUD;

	/// <summary>
	/// インベントリのUI
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> InventoryUIClass;
	UUserWidget* InventoryUI;

	/// <summary>
	/// HPバーのUI　
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HPUIClass;
	UUserWidget* HPUI;

};

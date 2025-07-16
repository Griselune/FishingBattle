// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WuBranch/GameState/FishingBattleGameState.h"
#include "DeathMatchGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeTimeDelegate, int, Min, int, Sec);

class UTimer;

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API ADeathMatchGameState : public AFishingBattleGameState
{
	GENERATED_BODY()
	
public:

	ADeathMatchGameState();

protected:

	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/// <summary>
	/// タイマーの時間が変更されたときのデリゲート
	/// </summary>
	UPROPERTY(BlueprintAssignable)
	FChangeTimeDelegate OnChangeTimeDelegate;

private:

	/// <summary>
	/// ゲーム開始時の処理
	/// </summary>
	UFUNCTION()
	void OnGameStateChanged(EGameStateList State);

	/// <summary>
	/// リザルト画面に切り替える
	/// </summary>
	void GotoResultMap();

#pragma region タイマー

	/// <summary>
	/// タイマーの作成
	/// </summary>
	void CreateTimer();

	/// <summary>
	/// タイマーの時間が変更されたときの処理(サーバー)
	/// </summary>
	/// <param name="TotalSec">全秒数</param>
	UFUNCTION()
	void Server_OnTimeChanged(float TotalSec);

	/// <summary>
	/// タイマーが終了したときの処理(サーバー)
	/// </summary>
	UFUNCTION()
	void Server_OnTimeFinished();

	/// <summary>
	/// タイマーの時間が変更されたことを通知する(クライアント)
	/// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void NotifyTimeChanged(int Min, int Sec);
	
	/// <summary>
	/// ゲームタイマー
	/// </summary>
	UPROPERTY()
	UTimer* GameTimer;
#pragma endregion


};

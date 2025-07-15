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

#pragma region タイマー

	/// <summary>
	/// タイマーの作成
	/// </summary>
	void CreateTimer();

	/// <summary>
	/// タイマーのカウントをバインド
	/// </summary>
	void BindTimerCount();

	/// <summary>
	/// タイマーの時間が変更されたときの処理(サーバー)
	/// </summary>
	/// <param name="TotalSec">全秒数</param>
	void Server_OnTimeChanged(float TotalSec);

	/// <summary>
	/// タイマーが終了したときの処理(サーバー)
	/// </summary>
	void Server_OnTimeFinished();

	/// <summary>
	/// タイマーの時間が変更されたときの処理(クライアント)
	/// </summary>
	UFUNCTION()
	void OnRep_Client_OnTimeChanged();

	/// <summary>
	/// タイマーの時間が変更されたことを通知する(クライアント)
	/// </summary>
	void NotifyTimeChanged();
	
	/// <summary>
	/// ゲームタイマー
	/// </summary>
	UTimer* GameTimer;

	/// <summary>
	/// タイマーの分
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Client_OnTimeChanged)
	int Min;

	/// <summary>
	/// タイマーの秒
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Client_OnTimeChanged)
	int Sec;
#pragma endregion


};

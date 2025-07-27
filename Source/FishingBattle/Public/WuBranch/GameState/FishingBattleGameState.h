// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameStateList.h"
#include "FishingBattleGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountDownDelegate, int, Num);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameStateChangedDelegate, EGameStateList, State);

class UTimer;

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API AFishingBattleGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	AFishingBattleGameState();

protected:

	virtual void BeginPlay();

public:

	// Called every frame
	virtual void Tick(float DeltaTime);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	/// <summary>
	/// ゲームの状態を変更
	/// </summary>
	/// <param name="NewState">新しい状態</param>
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_ChangeState(EGameStateList NewState);

	/// <summary>
	/// 今のゲームの状態をゲット
	/// </summary>
	/// <returns>ゲームの状態</returns>
	EGameStateList GetCurrentState() const;

	/// <summary>
	/// 各状態の初期化
	/// </summary>
	void InitState();

	/// <summary>
	/// ゲームは既に開始したか
	/// </summary>
	/// <returns>true: はい, false: いいえ</returns>
	bool IsStarted() const;

	/// <summary>
	/// ゲームは終了したか
	/// </summary>
	/// <returns>true: はい, false: いいえ</returns>
	bool IsFinished() const;

	/// <summary>
	/// ウントダウンの登録
	/// </summary>
	UPROPERTY(BlueprintAssignable)
	FCountDownDelegate OnCountDown;

	/// <summary>
	/// ゲーム状態が変更されたときの登録
	/// </summary>
	UPROPERTY(BlueprintAssignable)
	FGameStateChangedDelegate Client_OnGameStateChanged;
	UPROPERTY(BlueprintAssignable)
	FGameStateChangedDelegate Server_OnGameStateChanged;

#pragma region ゲーム状態
private:

	/// <summary>
	/// プレイヤー全員がマップにいるか
	/// </summary>
	/// <returns>true: はい,false: いいえ</returns>
	bool AreAllPlayersInMap();

	/// <summary>
	/// クライアントで状態が変更されたときの処理
	/// </summary>
	UFUNCTION()
	void OnRep_CurrentState();

	/// <summary>
	/// ゲーム状態が変更されたのを知らせる
	/// </summary>
	void NotifyStateChangedOnClient();
	void NotifyStateChangedOnServer();

	/// <summary>
	/// 今の状態
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentState)
	EGameStateList CurrentState;

#pragma endregion

#pragma region カウントダウン
private:
	/// <summary>
	/// タイマーの更新
	/// </summary>
	/// <param name="Sec">秒数</param>
	UFUNCTION()
	void OnTimerUpdate(float Sec);

	/// <summary>
	/// タイマーが終了した通知を受け
	/// </summary>
	UFUNCTION()
	void OnTimerFinished();

	/// <summary>
	/// カウントダウンのUIを更新
	/// </summary>
	/// <param name="Sec">秒数</param>
	UFUNCTION(NetMulticast, Reliable)
	void UpdateCountDownUI(int Sec);

	/// <summary>
	/// スタート前のカウントダウンタイマー
	/// </summary>
	UTimer* StartTimer;

	/// <summary>
	/// 前回のカウントダウン秒数
	/// </summary>
	int PreCountDownTime;
#pragma endregion

};

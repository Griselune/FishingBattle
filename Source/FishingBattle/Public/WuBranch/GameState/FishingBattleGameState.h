// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateList.h"
#include "FishingBattleGameState.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API AFishingBattleGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	AFishingBattleGameState();

protected:

	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	/// ウントダウンのデリゲート
	/// </summary>
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountDownDelegate, int, Num);

	UPROPERTY(BlueprintAssignable)
	FCountDownDelegate OnCountDown;

	/// <summary>
	/// ゲーム状態が変更されたときのデリゲート
	/// </summary>
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameStateChangedDelegate, EGameStateList, State);

	UPROPERTY(BlueprintAssignable)
	FGameStateChangedDelegate OnGameStateChanged;

private:

#pragma region ゲーム状態

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
	void NotifyStateChanged();

	/// <summary>
	/// 今の状態
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentState)
	EGameStateList CurrentState;

#pragma endregion

#pragma region カウントダウン
	/// <summary>
	/// カウントダウン
	/// </summary>
	/// <param name="DeltaTime">デルタタイム</param>
	void CountDown(float DeltaTime);

	/// <summary>
	/// カウントダウンのUIを更新
	/// </summary>
	/// <param name="Sec">秒数</param>
	void UpdateCountDownUI(int Sec);

	/// <summary>
	/// 開始前のカウントダウン秒数
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int StartCountDownSecond;

	/// <summary>
	/// カウントダウンの開始フラグ
	/// </summary>
	bool IsStartCountDown;

	/// <summary>
	/// カウントダウンの計算時間
	/// </summary>
	float CountDownTime;

	/// <summary>
	/// カウントダウンの終了時間
	/// 0でカウントダウン終了ですが、0の後にStartが表示するので-1を設定した
	/// </summary>
	float CountDownEndTime;

	/// <summary>
	/// 前回のカウントダウン秒数
	/// </summary>
	int PreCountDownTime;
#pragma endregion

};

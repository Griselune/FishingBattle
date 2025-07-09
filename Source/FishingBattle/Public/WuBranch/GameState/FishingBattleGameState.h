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

private:

	/// <summary>
	/// クライアントで状態が変更されたときの処理
	/// </summary>
	UFUNCTION()
	void OnRep_CurrentState();

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
	/// 今の状態
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentState)
	EGameStateList CurrentState;

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
	/// 
	/// </summary>
	int PreCountDownTime;
};

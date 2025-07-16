// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Timer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInitDelegate, float, Sec);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateDelegate, float, Sec);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFinishedDelegate);

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API UTimer : public UObject
{
	GENERATED_BODY()
	
public:

	// タイマーの状態
	enum class ETimerState : uint8
	{
		Init,
		Running,
		Paused,
		Finished
	};

	enum class ETimerType : uint8
	{
		CountDown,
		CountUp
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="WorldContext">ワールド</param>
	/// <param name="Start">開始秒数</param>
	/// <param name="End">終了秒数</param>
	/// <param name="TimerType">タイプ</param>
	/// <param name="DeltaTime">時間差</param>
	void Init(UWorld* WorldContext, float Start, float End, ETimerType TimerType, float TickTime);

	/// <summary>
	/// カウント開始
	/// </summary>
	void Start();

	/// <summary>
	/// カウント停止
	/// </summary>
	void Stop();

	/// <summary>
	/// 初期化完了の登録
	/// </summary>
	FInitDelegate OnInited;

	/// <summary>
	/// 終了の登録
	/// </summary>
	FFinishedDelegate OnFinished;

	/// <summary>
	/// 更新の登録
	/// </summary>
	FUpdateDelegate OnUpdated;

private:

	void Tick();

	/// <summary>
	/// カウントの実行
	/// </summary>
	void Count();

	/// <summary>
	/// 終了の確認
	/// </summary>
	bool IsFinished() const;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finish();

	/// <summary>
	/// 初期化の通知
	/// </summary>
	void NotifyInited();

	/// <summary>
	/// 更新の通知
	/// </summary>
	void NotifyUpdated();

	/// <summary>
	/// 終了の通知
	/// </summary>
	void NotifyFinished();

	/// <summary>
	/// 世界
	/// </summary>
	UWorld* World;

	FTimerHandle TimerHandle;

	/// <summary>
	/// 次のTickを実行するまでの時間差
	/// </summary>
	float DeltaTime;

	/// <summary>
	/// 実際カウントの値
	/// </summary>
	float Value;

	/// <summary>
	/// 終了時間
	/// </summary>
	float EndTime;

	/// <summary>
	/// 状態
	/// </summary>
	ETimerState State;

	/// <summary>
	/// タイプ
	/// </summary>
	ETimerType Type;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Object/Timer.h"

void UTimer::Init(UWorld* WorldContext, float Start, float End, ETimerType TimerType, float TickTime)
{
	if (!WorldContext)
	{
		UE_LOG(LogTemp, Warning, TEXT("WorldContext is null in UTimer::Init"));
		return;
	}

	// 確認
	if (Type == ETimerType::CountDown)
	{
		// カウントダウンの場合は、開始値が終了値より大きいことを確認
		if (Start <= End)
		{
			UE_LOG(LogTemp, Warning, TEXT("Start value must be greater than End value for CountDown timer in UTimer::Init"));
			return;
		}
	}
	else if (Type == ETimerType::CountUp)
	{
		// カウントアップの場合は、開始値が終了値より小さいことを確認
		if (Start >= End)
		{
			UE_LOG(LogTemp, Warning, TEXT("Start value must be less than End value for CountUp timer in UTimer::Init"));
			return;
		}
	}

	// 初期化
	World = WorldContext;
	Value = Start;
	EndTime = End;
	State = ETimerState::Init;
	Type = TimerType;
	DeltaTime = TickTime;

	// UIの更新
	NotifyInited();
}

void UTimer::Start()
{
	State = ETimerState::Running;
	// タイマーの状態を設定
	World->GetTimerManager().SetTimer(TimerHandle, this, &UTimer::Tick, this->DeltaTime, true);
}

void UTimer::Stop()
{
	State = ETimerState::Paused;
}

void UTimer::Tick()
{
	// 中止ではない
	if (State != ETimerState::Running)
		return;

	Count();

	if (IsFinished())
	{
		Finish();
	}
	
	NotifyUpdated();
}

void UTimer::Count()
{
	if (Type == ETimerType::CountUp)
	{
		Value += DeltaTime;
	}
	else if (Type == ETimerType::CountDown)
	{
		Value -= DeltaTime;
	}
}

bool UTimer::IsFinished() const
{
	// カウントアップ
	if (Type == ETimerType::CountUp)
		return Value >= EndTime;
	// カウントダウン
	else
		return Value <= EndTime;
}

void UTimer::Finish()
{
	NotifyFinished();
	State = ETimerState::Finished;
	World->GetTimerManager().ClearTimer(TimerHandle);
}

void UTimer::NotifyInited()
{
	if (OnInited.IsBound())
	{
		OnInited.Broadcast(Value);
	}
}

void UTimer::NotifyUpdated()
{
	if (OnUpdated.IsBound())
	{
		OnUpdated.Broadcast(Value);
	}
}

void UTimer::NotifyFinished()
{
	if (OnFinished.IsBound())
	{
		OnFinished.Broadcast();
	}
}

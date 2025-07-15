// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/GameState/DeathMatchGameState.h"
#include <Net/UnrealNetwork.h>
#include "WuBranch/Object/Timer.h"

void ADeathMatchGameState::BeginPlay()
{
	Super::BeginPlay();

	CreateTimer();
}

void ADeathMatchGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ADeathMatchGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// レプリケーションするプロパティを追加
	DOREPLIFETIME(ADeathMatchGameState, Min);
	DOREPLIFETIME(ADeathMatchGameState, Sec);
}

void ADeathMatchGameState::CreateTimer()
{
	// サーバー側でのみ実行
	if (!HasAuthority())
		return;

	GameTimer = NewObject<UTimer>(this, UTimer::StaticClass());
	// 先に初期化終了のバインドを行う
	GameTimer->OnInited.AddDynamic(this, &ADeathMatchGameState::Server_OnTimeChanged);
	// タイマーの初期化
	// 10分のカウントダウンを設定(今後はプレイヤーが選んだ時間で代入)
	GameTimer->Init(GetWorld(), 10 * 60, 0, UTimer::ETimerType::CountDown, 1.0f);
	BindTimerCount();
}

void ADeathMatchGameState::BindTimerCount()
{
	// サーバー側でのみ実行
	if (!HasAuthority())
		return;

	GameTimer->OnUpdated.AddDynamic(this, &ADeathMatchGameState::Server_OnTimeChanged);
	GameTimer->OnFinished.AddDynamic(this, &ADeathMatchGameState::Server_OnTimeFinished);
}

void ADeathMatchGameState::Server_OnTimeChanged(float TotalSec)
{
	// サーバー側でのみ実行
	if (!HasAuthority())
		return;

	// 秒数を分と秒に変換
	Min = (int)TotalSec / 60;
	Sec = (int)TotalSec % 60;
}

void ADeathMatchGameState::Server_OnTimeFinished()
{
	Server_ChangeState(EGameStateList::Finished);
}

void ADeathMatchGameState::OnRep_Client_OnTimeChanged()
{
	NotifyTimeChanged();
}

void ADeathMatchGameState::NotifyTimeChanged()
{
	if (OnChangeTimeDelegate.IsBound())
	{
		OnChangeTimeDelegate.Broadcast(Min, Sec);
	}
}

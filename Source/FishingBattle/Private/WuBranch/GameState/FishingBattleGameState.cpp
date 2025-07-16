// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/GameState/FishingBattleGameState.h"
#include <Net/UnrealNetwork.h>

AFishingBattleGameState::AFishingBattleGameState()
	: IsStartCountDown(false)
	, CountDownTime(0.0f)
	, CountDownEndTime(-1.0f)
	, PreCountDownTime(0)
{
	// Set this game state to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CurrentState = EGameStateList::Init;
}

void AFishingBattleGameState::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
		Server_ChangeState(EGameStateList::CheckPlayerState);
}

void AFishingBattleGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// カウントダウンの処理
	if (IsStartCountDown)
	{
		CountDown(DeltaTime);
	}

	if (AreAllPlayersInMap())
	{
		Server_ChangeState(EGameStateList::BeforeStart);
	}
}

void AFishingBattleGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// レプリケーションするプロパティを追加
	DOREPLIFETIME(AFishingBattleGameState, CurrentState);
}

void AFishingBattleGameState::Server_ChangeState_Implementation(EGameStateList NewState)
{
	// サーバー側で状態を変更
	if (HasAuthority())
	{
		if (CurrentState == NewState) 
			return;

		CurrentState = NewState;
		NotifyStateChangedOnServer();
	}
}

bool AFishingBattleGameState::IsStarted() const
{
	// 開始、終了状態以上なら開始済み
	return CurrentState >= EGameStateList::Started;
}

bool AFishingBattleGameState::IsFinished() const
{
	return CurrentState >= EGameStateList::Finished;
}

bool AFishingBattleGameState::AreAllPlayersInMap()
{
	// サーバー側でのみ実行
	if (!HasAuthority())
		return false;

	// プレイヤーの状態を確認する段階にいる
	if (CurrentState != EGameStateList::CheckPlayerState)
		return false;

	// プレイヤーの状態を確認する処理
	for (APlayerState* PlayerState : PlayerArray)
	{
		// 未完成、プレイヤーステートからプレイヤーがマップにいるかを確認
		if (!PlayerState)
		{
			return false;
		}
	}
	return true;
}

void AFishingBattleGameState::OnRep_CurrentState()
{
	switch (CurrentState)
	{
		case EGameStateList::BeforeStart:
			CountDownTime = StartCountDownSecond;
			PreCountDownTime = 0;
			IsStartCountDown = true;
			break;
		case EGameStateList::Started:
			break;
		case EGameStateList::Finished:
			break;
	}
	NotifyStateChangedOnClient();
}

void AFishingBattleGameState::NotifyStateChangedOnClient()
{
	if (Client_OnGameStateChanged.IsBound())
	{
		Client_OnGameStateChanged.Broadcast(CurrentState);
	}
}

void AFishingBattleGameState::NotifyStateChangedOnServer()
{
	if (Server_OnGameStateChanged.IsBound())
	{
		Server_OnGameStateChanged.Broadcast(CurrentState);
	}
}

void AFishingBattleGameState::CountDown(float DeltaTime)
{
	CountDownTime -= DeltaTime;
	int Sec = FMath::CeilToInt(CountDownTime);
	if (Sec != PreCountDownTime)
	{
		UpdateCountDownUI(Sec);
		PreCountDownTime = Sec;
	}
	if (Sec <= CountDownEndTime)
	{
		IsStartCountDown = false;
		Server_ChangeState(EGameStateList::Started);
	}
}

void AFishingBattleGameState::UpdateCountDownUI(int Sec)
{
	if (OnCountDown.IsBound())
	{
		OnCountDown.Broadcast(Sec);
	}
}

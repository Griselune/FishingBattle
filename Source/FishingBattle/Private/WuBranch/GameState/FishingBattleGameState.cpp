// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/GameState/FishingBattleGameState.h"
#include <Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"
#include <WuBranch/UI/BaseHUD.h>

AFishingBattleGameState::AFishingBattleGameState()
	: IsStartCountDown(false)
	, CountDownTime(0.0f)
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

	// テスト用
	CurrentState = EGameStateList::CheckPlayerState;
	//OnRep_CurrentState();
}

void AFishingBattleGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// カウントダウンの処理
	if (IsStartCountDown)
	{
		CountDown(DeltaTime);
	}

	if (CurrentState == EGameStateList::CheckPlayerState)
	{
		// プレイヤーの状態を確認する処理
		//TArray<APlayerState*> PlayerStates = PlayerArray;
		//bool AllPlayersReady = true;
		//for (APlayerState* PlayerState : PlayerStates)
		//{
		//	if (!PlayerState || !PlayerState->bIsReady) // bIsReadyはプレイヤーが準備完了かどうかのフラグ
		//	{
		//		AllPlayersReady = false;
		//		break;
		//	}
		//}
		//if (AllPlayersReady)
		//{
		//	Server_ChangeState(EGameStateList::BeforeStart);
		//}
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
	if (HasAuthority())
	{
		// サーバー側で状態を変更
		if (CurrentState == NewState) return;
		CurrentState = NewState;
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
	if (HasAuthority())
	{
		PlayerArray;
		return false;
	}
	else
		return false;
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
	if (Sec <= 0)
	{
		IsStartCountDown = false;
	}
}

void AFishingBattleGameState::UpdateCountDownUI(int Sec)
{
	OnCountDown.Broadcast(Sec);
}

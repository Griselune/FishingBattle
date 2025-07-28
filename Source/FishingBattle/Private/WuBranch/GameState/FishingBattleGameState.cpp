// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/GameState/FishingBattleGameState.h"
#include <Net/UnrealNetwork.h>
#include "WuBranch/Object/Timer.h"
#include <PrinzBranch/LANGameInstance.h>

AFishingBattleGameState::AFishingBattleGameState()
	: PreCountDownTime(0)
{
	// Set this game state to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CurrentState = EGameStateList::Init;
}

void AFishingBattleGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Server_ChangeState(EGameStateList::CheckPlayerState);	
	}
}

void AFishingBattleGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		InitState();
		NotifyStateChangedOnServer();
	}
}

EGameStateList AFishingBattleGameState::GetCurrentState() const
{
	return CurrentState;
}

void AFishingBattleGameState::InitState()
{
	// サーバー側でのみ実行
	if (!HasAuthority())
		return;

	// 各状態の初期化
	switch (CurrentState)
	{
	case EGameStateList::BeforeStart:
		PreCountDownTime = 0;
		StartTimer = NewObject<UTimer>(this, UTimer::StaticClass());
		StartTimer->OnInited.AddDynamic(this, &AFishingBattleGameState::OnTimerUpdate);
		StartTimer->Init(GetWorld(), 5.0f, 0.0f, UTimer::ETimerType::CountDown, 0.1f);
		StartTimer->OnUpdated.AddDynamic(this, &AFishingBattleGameState::OnTimerUpdate);
		StartTimer->OnFinished.AddDynamic(this, &AFishingBattleGameState::OnTimerFinished);
		StartTimer->Start();
		break;
	case EGameStateList::Started:
		break;
	case EGameStateList::Finished:
		break;
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
	ULANGameInstance* GInstance = GetGameInstance<ULANGameInstance>();
	if (!GInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Instance is nullptr!"));
		return false;
	}
	int PlayerNums = GInstance->GISessionCurrentPlayers;
	UE_LOG(LogTemp, Warning, TEXT("Player Count in Map: %d, Need: %d"), PlayerArray.Num(), PlayerNums);
	return PlayerNums == PlayerArray.Num();
}

void AFishingBattleGameState::OnRep_CurrentState()
{
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

void AFishingBattleGameState::OnTimerUpdate(float Sec)
{
	int Second = FMath::CeilToInt(Sec);
	if (PreCountDownTime != Second)
	{
		UpdateCountDownUI(Second);
		PreCountDownTime = Second;
	}
}

void AFishingBattleGameState::OnTimerFinished()
{
	if (HasAuthority())
	{
		Server_ChangeState(EGameStateList::Started);
	}
}

void AFishingBattleGameState::UpdateCountDownUI_Implementation(int Sec)
{
	if (OnCountDown.IsBound())
	{
		OnCountDown.Broadcast(Sec);
	}
}

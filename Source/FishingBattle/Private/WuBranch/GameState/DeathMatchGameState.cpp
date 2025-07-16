// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/GameState/DeathMatchGameState.h"
#include <Net/UnrealNetwork.h>

void ADeathMatchGameState::BeginPlay()
{
	Super::BeginPlay();

	BindTimerCount();
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

void ADeathMatchGameState::BindTimerCount()
{
	// サーバー側でのみ実行
	if (!HasAuthority())
		return;

	//GetDefaultGameMode<>();
}

void ADeathMatchGameState::Server_OnTimeChanged(int TotalSec)
{
	// サーバー側でのみ実行
	if (!HasAuthority())
		return;

	// 秒数を分と秒に変換
	Min = TotalSec / 60;
	Sec = TotalSec % 60;
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

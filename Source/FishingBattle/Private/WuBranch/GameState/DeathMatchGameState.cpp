// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/GameState/DeathMatchGameState.h"
#include <Net/UnrealNetwork.h>
#include "WuBranch/Object/Timer.h"
#include <Kismet/GameplayStatics.h>
#include <PrinzBranch/LANGameInstance.h>

ADeathMatchGameState::ADeathMatchGameState()
{
	// Set this game state to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADeathMatchGameState::BeginPlay()
{
	Super::BeginPlay();

	// サーバー側のみ実行
	if (HasAuthority())
	{
		Server_OnGameStateChanged.AddDynamic(this, &ADeathMatchGameState::OnGameStateChanged);
		CreateTimer();
	}
	// クライアント側のみ実行
	else
	{
		Client_OnGameStateChanged.AddDynamic(this, &ADeathMatchGameState::OnGameStateChanged);
	}
}

void ADeathMatchGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ADeathMatchGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// レプリケーションするプロパティを追加
}

void ADeathMatchGameState::OnGameStateChanged(EGameStateList State)
{
	// サーバー側のみ実行
	if(HasAuthority())
	{
		UE_LOG(LogTemp, Display, TEXT("Server Get GameState Changed: %d"), (int)State);
		if (State == EGameStateList::Started)
		{
			GameTimer->Start();
		}
		else if (State == EGameStateList::Finished)
		{
			// 複数のPCで確認する前にサーバ側は一旦リザルト画面に行かない
			// 一つのPCで複数人を模擬した場合はUnreal Engineはクラッシュする
			OnGameFinished();
		}
	}
}

void ADeathMatchGameState::OnGameFinished()
{
	// 3秒後に切り替える
	FTimerHandle ChangeMapHandler;
	UWorld* world = GetWorld();
	GetWorld()->GetTimerManager().SetTimer(ChangeMapHandler, this, &ADeathMatchGameState::Changelevel, 1.0f, false, 3.0f);
}

void ADeathMatchGameState::Changelevel()
{
	GetWorld()->ServerTravel("/Game/WuBranch/Maps/ResultMap?listen", true);
}

void ADeathMatchGameState::CreateTimer()
{
	// サーバー側のみ実行
	if (!HasAuthority())
		return;

	GameTimer = NewObject<UTimer>(this, UTimer::StaticClass());
	// 先に初期化終了のバインドを行う
	GameTimer->OnInited.AddDynamic(this, &ADeathMatchGameState::Server_OnTimeChanged);
	// タイマーの初期化
	// デフォルトは３分
	int Time = 3;
	if (ULANGameInstance* GameInstance = GetGameInstance<ULANGameInstance>())
	{
		Time = GameInstance->GISessionTimeLimit;
	}
	UE_LOG(LogTemp, Warning, TEXT("Game Time: %d"), Time);
	GameTimer->Init(GetWorld(), Time * 60.0f, 0.0f, UTimer::ETimerType::CountDown, 1.0f);
	//GameTimer->Init(GetWorld(), 10.0f, 0.0f, UTimer::ETimerType::CountDown, 1.0f);
	// タイマーの更新と終了イベントを登録
	GameTimer->OnUpdated.AddDynamic(this, &ADeathMatchGameState::Server_OnTimeChanged);
	GameTimer->OnFinished.AddDynamic(this, &ADeathMatchGameState::Server_OnTimeFinished);
}

void ADeathMatchGameState::Server_OnTimeChanged(float TotalSec)
{
	// サーバー側のみ実行
	if (!HasAuthority())
		return;

	// 秒数を分と秒に変換
	int Min = (int)TotalSec / 60;
	int Sec = (int)TotalSec % 60;
	NotifyTimeChanged(Min, Sec);
}

void ADeathMatchGameState::Server_OnTimeFinished()
{
	Server_ChangeState(EGameStateList::Finished);
}

void ADeathMatchGameState::NotifyTimeChanged_Implementation(int Min, int Sec)
{
	if (OnChangeTimeDelegate.IsBound())
	{
		OnChangeTimeDelegate.Broadcast(Min, Sec);
	}
}

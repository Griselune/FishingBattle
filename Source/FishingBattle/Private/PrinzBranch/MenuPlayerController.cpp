// Fill out your copyright notice in the Description page of Project Settings.

#include "PrinzBranch/MenuPlayerController.h"
#include "PrinzBranch/GS_MenuGameState.h"
#include "PrinzBranch/LANGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PrinzBranch/PS_MenuPlayerState.h"




#pragma region Initialize
void AMenuPlayerController::SessionDataInitialize()
{
	if (!MatchMakingWidget) return;

	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
	FString GameMode = GS->GSisDeathMatch ? "Death Match" : "Battle Royale";

	MatchMakingWidget->SetTextReadyPlayers(GS->GSReadyPlayers, GS->GSCurrentPlayers);
	MatchMakingWidget->SetTextPlayerMax(FString::FromInt(GS->GSSessionPlayerLimit));
	MatchMakingWidget->SetTextSessionName(GS->GSSessionName);
	MatchMakingWidget->SetTextSessionGameMode(GameMode);
	MatchMakingWidget->SetTextSessionTimeLimit(FString::FromInt(GS->GSSessionTimeLimit));
}
void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController()) {
		SessionDataInitialize();
	}
}
#pragma endregion

//サーバーのみ
void AMenuPlayerController::Server_SetPlayerReady_Implementation(bool IsPlayerReady)
{
	// RPCを呼び出したクライアントからPSを取得
	APS_MenuPlayerState* PS = GetPlayerState<APS_MenuPlayerState>();
	if (PS) {
		PS->PSisPlayerReady = IsPlayerReady; //サーバーがクライアント用の値を設定

		PS->OnRep_PlayerReady(); //サーバーUIを強制更新
	}

	//PlayNowボタンの色をサーバー側のみで変更
	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
	if (GS) {
		bool bAllReady = GS->Server_CheckAllPlayersReady();
		//ホストのボタンUIのみ更新
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
			AMenuPlayerController* PC = Cast<AMenuPlayerController>(*It);
			if (PC && PC->IsLocalController() && PC->HasAuthority() && PC->MatchMakingWidget) { //サーバーの場合
				PC->MatchMakingWidget->SetButtonPlayNowColor(bAllReady);
			}

			if (PC && PC->MatchMakingWidget) {
				//現在のプレイヤー数と準備完了プレイヤー数を更新
				PC->MatchMakingWidget->SetTextReadyPlayers(GS->GSReadyPlayers, GS->GSCurrentPlayers);
			}
		}
	}
}
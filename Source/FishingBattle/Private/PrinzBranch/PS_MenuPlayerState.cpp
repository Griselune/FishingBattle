// Fill out your copyright notice in the Description page of Project Settings.


#include "PrinzBranch/PS_MenuPlayerState.h"
#include <Net/UnrealNetwork.h>
#include "PrinzBranch/LANGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PrinzBranch/MenuPlayerController.h"
#include "PrinzBranch/GS_MenuGameState.h"
#include "PrinzBranch/MatchMakingWidget.h"

/// <summary>
/// 各レベルで各プレヤーの名前を更新する
/// </summary>
void APS_MenuPlayerState::BeginPlay()
{
	Super::BeginPlay();

	//Localクライアントのみ
	AMenuPlayerController* PC = Cast<AMenuPlayerController>(GetOwner());
	if (PC && PC->IsLocalController())
	{
		if (PC->MatchMakingWidget) {
			AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
			if (GS) {
				PC->MatchMakingWidget->SetTextReadyPlayers(GS->GSReadyPlayers, GS->GSCurrentPlayers);
			}
		}
		ULANGameInstance* GI = GetGameInstance<ULANGameInstance>();
		if (GI)
		{
			Server_SetPlayerName(GI->GIPlayerName);
		}
	}
}

void APS_MenuPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// レプリケーションするプロパティを追加
	DOREPLIFETIME(APS_MenuPlayerState, PSPlayerName);
	DOREPLIFETIME(APS_MenuPlayerState, PSisPlayerReady);
}

void APS_MenuPlayerState::OnRep_MenuPlayerName()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep trigger - Name: %s"), *PSPlayerName);
}

void APS_MenuPlayerState::OnRep_PlayerReady()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep trigger - Player Ready = %s"), PSisPlayerReady ? TEXT("true") : TEXT("false"));

	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();

	// Update local UI
	AMenuPlayerController* PC = Cast<AMenuPlayerController>(GetOwner());
	if (PC && PC->MatchMakingWidget)
	{
		PC->MatchMakingWidget->SetButtonReadyColor(PSisPlayerReady);
		if (GS) {
			PC->MatchMakingWidget->SetTextReadyPlayers(GS->GSReadyPlayers, GS->GSCurrentPlayers);
		}
	}
}



void APS_MenuPlayerState::Server_SetPlayerName_Implementation(const FString& NewName)
{
	PSPlayerName = NewName;
}
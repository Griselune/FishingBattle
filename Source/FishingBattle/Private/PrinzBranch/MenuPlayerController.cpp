// Fill out your copyright notice in the Description page of Project Settings.

#include "PrinzBranch/MenuPlayerController.h"
#include "PrinzBranch/GS_MenuGameState.h"
#include "PrinzBranch/LANGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PrinzBranch/PS_MenuPlayerState.h"


//server only
void AMenuPlayerController::Server_SetPlayerReady_Implementation(bool IsPlayerReady)
{
	//get PS from the client that calls the RPC
	APS_MenuPlayerState* PS = GetPlayerState<APS_MenuPlayerState>();
	if (PS) {
		PS->PSisPlayerReady = IsPlayerReady; //server sets the value for the client

		PS->OnRep_PlayerReady(); //force server UI update
	}

	//change PlayNow button color server only
	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
	if (GS)
	{
		bool bAllReady = GS->Server_CheckAllPlayersReady();

		// Only update host’s button UI
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			AMenuPlayerController* PC = Cast<AMenuPlayerController>(*It);
			if (PC && PC->IsLocalController() && PC->HasAuthority() && PC->MatchMakingWidget) //If server
			{
				PC->MatchMakingWidget->SetButtonPlayNowColor(bAllReady);
			}

			if (PC->MatchMakingWidget) {
				//update all numbers
				PC->MatchMakingWidget->SetTextReadyPlayers(GS->GSReadyPlayers, GS->GSCurrentPlayers);
			}
		}
	}
}


#pragma region Initialize
void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
	if (MatchMakingWidget && IsLocalController()) {
		MatchMakingWidget->SetTextReadyPlayers(GS->GSReadyPlayers, GS->GSCurrentPlayers);
	}
}
#pragma endregion
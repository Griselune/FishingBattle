// Fill out your copyright notice in the Description page of Project Settings.


#include "PrinzBranch/PS_MenuPlayerState.h"
#include <Net/UnrealNetwork.h>
#include "PrinzBranch/LANGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PrinzBranch/MenuPlayerController.h"
#include "PrinzBranch/GS_MenuGameState.h"
#include "PrinzBranch/MatchMakingWidget.h"


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
	UE_LOG(LogTemp, Warning, TEXT("OnRep trigger - Player Ready"));

	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
	GS->Server_UpdateAllCurrentPlayers();
	GS->Server_UpdateAllReadyButtonColor();

	//AMenuPlayerController* PC = Cast<AMenuPlayerController>(GetOwner());
	//if (Server_CheckAllPlayersReady()) {
	//	if (PC) {
	//		if (PC->MatchMakingWidget) {
	//			PC->MatchMakingWidget->SetButtonColor(true);
	//		}
	//	}
	//}
	//else {
	//	if (PC) {
	//		if (PC->MatchMakingWidget) {
	//			PC->MatchMakingWidget->SetButtonColor(false);
	//		}
	//	}
	//}
}

//***MOVED TO MENUGAMESTATE***
//bool APS_MenuPlayerState::Server_CheckAllPlayersReady()
//{
//	bool bAllReady = true;
//	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
//
//	for (APlayerState* PS : GS->PlayerArray)
//	{
//		APS_MenuPlayerState* MyPS = Cast<APS_MenuPlayerState>(PS);
//		if (MyPS && !MyPS->PSisPlayerReady)
//		{
//			bAllReady = false;
//			break;
//		}
//	}
//
//	if (bAllReady)
//	{
//		UE_LOG(LogTemp, Log, TEXT("✅ All players are ready!"));
//		// Example: Tell GameMode to start the match
//		// AMyGameMode* GM = GetWorld()->GetAuthGameMode<AMyGameMode>();
//		// if (GM) GM->StartMatch();
//		return true;
//	}
//	return false;
//}



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
		ULANGameInstance* GI = GetGameInstance<ULANGameInstance>();
		if (GI)
		{
			Server_SetPlayerName(GI->GIPlayerName);
		}
	}
}


void APS_MenuPlayerState::Server_SetPlayerName_Implementation(const FString& NewName)
{
	PSPlayerName = NewName;
}


//***MOVED TO MENUPLAYERCONTROLLER***
//void APS_MenuPlayerState::Server_SetPlayerReady_Implementation(bool IsPlayerReady)
//{
//	PSisPlayerReady = IsPlayerReady;
//
//
//	AMenuPlayerController* PC = Cast<AMenuPlayerController>(GetOwner());
//	if (Server_CheckAllPlayersReady()) {
//		if (PC) {
//			if (PC->MatchMakingWidget) {
//				PC->MatchMakingWidget->SetButtonColor(true);
//			}
//		}
//	}
//	else {
//		if (PC) {
//			if (PC->MatchMakingWidget) {
//				PC->MatchMakingWidget->SetButtonColor(false);
//			}
//		}
//	}
//}




//void APS_MenuPlayerState::GetDataFromServer_Implementation(AController* NewPlayer)
//{
//	//	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
//	ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//	AGS_MenuGameState* GS = Cast<AGS_MenuGameState>(GetWorld()->GetGameState());
//
//	APlayerState* PS = NewPlayer->GetPlayerState<APlayerState>();
//
//	// server
//	//if(HasAuthority())
//
//
//	UE_LOG(LogTemp, Warning, TEXT("Server Send Session Name"));
//	GS->SetSessionName(GS->GSSessionName);
//
//	//	GIclient->GIPlayerList.Append(GIserver->GIPlayerList);
//	//	UE_LOG(LogTemp, Warning, TEXT("GIserver last name : %s"), *GIserver->GIPlayerList[0]);
//
//		//Gets player list from server
//	UE_LOG(LogTemp, Warning, TEXT("GI PlayerList: %d"), GI->GIPlayerList.Num());
	//for (auto& list : GI->GIPlayerList) {
	//	AddPlayerToList(list.Key, list.Value);  //crash
	//	UE_LOG(LogTemp, Warning, TEXT("Get Player list from Server"));
	//}

	////Client adds it's own name in the list
	//UE_LOG(LogTemp, Warning, TEXT("GI2 PlayerList: %d"), GIclient->GIPlayerList.Num());
	//if (GIclient)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Add Client name in the Player List"));
	//	AddPlayerToList(PS, GIclient->GIPlayerName);
	//}
	//UE_LOG(LogTemp, Warning, TEXT("GI server PlayerList: %d"), GIserver->GIPlayerList.Num());
//}

#pragma region Player List Functions
/// <summary>
/// プレヤーのリストの管理。各プレヤーのID（APlayerState）と名前を記入する。
/// </summary>
//void APS_MenuPlayerState::AddPlayerToList_Implementation(APlayerState* PS, const FString& PName)
//{
//	ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//	if (PS && GI) {
//		GI->GIPlayerList.Add(PS, PName);
//		GSPlayerList.Add(PS, PName);
//		UE_LOG(LogTemp, Warning, TEXT("AddPlayerToList() executed"));
//	}
//}
//
//void AGS_MenuGameState::RemovePlayerFromList_Implementation(APlayerState* PS)
//{
//	UE_LOG(LogTemp, Warning, TEXT("RemovePlayerFromList() executed"));
//	for (auto& list : GSPlayerList) {
//		if (list.Key == PS) {
//			GSPlayerList.Remove(list.Key);  //リストからプレヤーを取り消す
//			UE_LOG(LogTemp, Warning, TEXT("A player has been removed"));
//			break;
//		}
//	}
//
//}


#pragma endregion
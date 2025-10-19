// Fill out your copyright notice in the Description page of Project Settings.


#include "PrinzBranch/GS_MenuGameState.h"
#include <Net/UnrealNetwork.h>
#include "PrinzBranch/LANGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PrinzBranch/MenuPlayerController.h"
#include "PrinzBranch/PS_MenuPlayerState.h"

#pragma region Initialize
void AGS_MenuGameState::BeginPlay()
{
	Super::BeginPlay();


}
#pragma endregion

#pragma region Replication
void AGS_MenuGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// レプリケーションするプロパティを追加
	DOREPLIFETIME(AGS_MenuGameState, GSSessionName);
	DOREPLIFETIME(AGS_MenuGameState, GSReadyPlayers);
	DOREPLIFETIME(AGS_MenuGameState, GSPlayerName);
}

void AGS_MenuGameState::OnRep_SessionData()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep trigger - Name: %s"), *GSSessionName);
}
#pragma endregion

void AGS_MenuGameState::AddPlayerToList(APlayerState* PS)
{
	if (!HasAuthority() || !PS)
		return;

	APS_MenuPlayerState* MyPS = Cast<APS_MenuPlayerState>(PS);
	if (MyPS)
	{
		GSPlayerList.Add(MyPS, MyPS->PSPlayerName);
		UE_LOG(LogTemp, Warning, TEXT("Player added to GS list: %s"), *MyPS->PSPlayerName);
	}
}












//void AGS_MenuGameState::GetDataFromServer_Implementation(AController* NewPlayer)
//{
////	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
//	ULANGameInstance* GIserver = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//
//	APlayerState* PS = NewPlayer->GetPlayerState<APlayerState>();
//	ULANGameInstance* GIclient = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(NewPlayer->GetWorld()));
//
//	// server
//	//if(HasAuthority())
//		
//
//	UE_LOG(LogTemp, Warning, TEXT("Server Send Session Name"));
//	SetSessionName(GSSessionName);
//
////	GIclient->GIPlayerList.Append(GIserver->GIPlayerList);
////	UE_LOG(LogTemp, Warning, TEXT("GIserver last name : %s"), *GIserver->GIPlayerList[0]);
//
//	//Gets player list from server
//	UE_LOG(LogTemp, Warning, TEXT("GI PlayerList: %d"), GIserver->GIPlayerList.Num());
//	for (auto& list : GIserver->GIPlayerList) {
//		AddPlayerToList(list.Key, list.Value);  //crash
//		UE_LOG(LogTemp, Warning, TEXT("Get Player list from Server"));
//	}
//
//	//Client adds it's own name in the list
//	UE_LOG(LogTemp, Warning, TEXT("GI2 PlayerList: %d"), GIclient->GIPlayerList.Num());
//	if (GIclient)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Add Client name in the Player List"));
//		AddPlayerToList(PS, GIclient->GIPlayerName);
//	}
//	UE_LOG(LogTemp, Warning, TEXT("GI server PlayerList: %d"), GIserver->GIPlayerList.Num());
//}


#pragma region Setter
void AGS_MenuGameState::SetSessionName_Implementation(const FString& Name)
{
	GSSessionName = Name;
	UE_LOG(LogTemp, Warning, TEXT("Get Name: %s"), *GSSessionName);
}

//void AGS_MenuGameState::SetSessionPassword_Implementation(const FString& Password)
//{
//	GSSessionPassword= Password;
//	UE_LOG(LogTemp, Warning, TEXT("Get Name: %s"), *GSSessionPassword);
//}

void AGS_MenuGameState::SetSessionPlayerLimit_Implementation(const int32& PlayerLimit)
{
	GSSessionPlayerLimit = PlayerLimit;
	UE_LOG(LogTemp, Warning, TEXT("Get Name: %d"), GSSessionPlayerLimit);
}

void AGS_MenuGameState::SetPlayerName_Implementation(const FString& PlayerName)   //name
{
	GSPlayerName = PlayerName;
	UE_LOG(LogTemp, Warning, TEXT("Get Name: %s"), *GSPlayerName);
}

void AGS_MenuGameState::SetSessionTimeLimit_Implementation(const int32& TimeLimit)
{
	GSSessionTimeLimit = TimeLimit;
	UE_LOG(LogTemp, Warning, TEXT("Get Name: %d"), GSSessionTimeLimit);
}

void AGS_MenuGameState::SetSessionIsDeathMatch_Implementation(bool isDM)
{
	GSisDeathMatch = isDM;
	if (GSisDeathMatch) {
		UE_LOG(LogTemp, Warning, TEXT("Get mode: Deathmatch"));
	}
}

void AGS_MenuGameState::SetSessionIsBattleRoyale_Implementation(bool isBR)
{
	GSisBattleRoyale = isBR;
	if (GSisBattleRoyale) {
		UE_LOG(LogTemp, Warning, TEXT("Get mode: Battleroyale"));
	}
}
#pragma endregion

#pragma region Player List Functions
/// <summary>
/// プレヤーのリストの管理。各プレヤーのID（APlayerState）と名前を記入する。
/// </summary>
//void AGS_MenuGameState::AddPlayerToList_Implementation(APlayerState* PS, const FString& PName)
//{
//	ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//	if (PS && GI) {
//		GI->GIPlayerList.Add(PS, PName);
//		GSPlayerList.Add(PS, PName);
//		UE_LOG(LogTemp, Warning, TEXT("AddPlayerToList() executed"));
//	}
//}

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

#pragma region Ready players functions
//void AGS_MenuGameState::HostAddReadyPlayer_Implementation()
//{
//	GSReadyPlayers += 1;
//}
//
//void AGS_MenuGameState::HostRemoveReadyPlayer_Implementation()
//{
//	GSReadyPlayers -=1 ;
//}
#pragma endregion
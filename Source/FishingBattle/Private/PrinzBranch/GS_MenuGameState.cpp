// Fill out your copyright notice in the Description page of Project Settings.


#include "PrinzBranch/GS_MenuGameState.h"
#include <Net/UnrealNetwork.h>

void AGS_MenuGameState::BeginPlay()
{
	//// client
	//if (!HasAuthority())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Client Ask Server Data"));
	//	GetDataFromServer();
	//}
}

void AGS_MenuGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// レプリケーションするプロパティを追加
	DOREPLIFETIME(AGS_MenuGameState, GSSessionName);
}

void AGS_MenuGameState::OnRep_SessionData()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep trigger - Name: %s"), *GSSessionName);
}

//void AGS_MenuGameState::GetDataFromServer_Implementation()
//{
//		UE_LOG(LogTemp, Warning, TEXT("Server Send Data"));
//		SendData();
//}

void AGS_MenuGameState::SendData()
{
	UE_LOG(LogTemp, Warning, TEXT("Server Send Session Name"));
	SetSessionName(GSSessionName);
	SetSessionPassword(GSSessionPassword);
	SetSessionPlayerLimit(GSSessionPlayerLimit);
	SetPlayerName(GSPlayerName);
	SetSessionTimeLimit(GSSessionTimeLimit);
	SetSessionIsDeathMatch(GSisDeathMatch);
	SetSessionIsBattleRoyale(GSisBattleRoyale);
	SetPlayerName(GSPlayerName);								 //should be Tmap
}

void AGS_MenuGameState::SetSessionName_Implementation(const FString& Name)
{
	GSSessionName = Name;
	UE_LOG(LogTemp, Warning, TEXT("Get Name: %s"), *GSSessionName);
}

void AGS_MenuGameState::SetSessionPassword_Implementation(const FString& Password)
{
	GSSessionPassword= Password;
	UE_LOG(LogTemp, Warning, TEXT("Get Name: %s"), *GSSessionPassword);
}

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


/// <summary>
/// プレヤーのリストの管理。各プレヤーのID（APlayerController）と名前を記入する。
/// </summary>
void AGS_MenuGameState::AddPlayerToList_Implementation(APlayerController* PC, const FString& PName)
{
	GSPlayerList.Add(PC, PName);
}

void AGS_MenuGameState::RemovePlayerFromList_Implementation(APlayerController* PC)
{
	for (auto& list : GSPlayerList) {
		if (list.Key == PC) {
			GSPlayerList.Remove(list.Key);  //リストからプレヤーを取り消す
			break;
		}
	}
}

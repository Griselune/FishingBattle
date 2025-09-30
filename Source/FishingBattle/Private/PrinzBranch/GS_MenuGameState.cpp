// Fill out your copyright notice in the Description page of Project Settings.


#include "PrinzBranch/GS_MenuGameState.h"
#include <Net/UnrealNetwork.h>
#include "PrinzBranch/LANGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PrinzBranch/MenuPlayerController.h"

void AGS_MenuGameState::BeginPlay()
{
	////Server -- Serverは自身の名前を先にプレヤーリストに入れる
	//if (HasAuthority())
	//{
	////	ULANGameInstance* GI = UGameplayStatics::GetGameInstance(this)->GetSubsystem<ULANGameInstance>();

	//	ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//	AMenuPlayerController* PC = GetWorld()->GetFirstPlayerController<AMenuPlayerController>();
	//	if (GI && PC)
	//	{
	//		AddPlayerToList(PC, GI->GIPlayerName);
	//	}
	//}
}

void AGS_MenuGameState::HostAddPlayerList_Implementation()
{
	if (HasAuthority()) {
		//Server -- Serverは自身の名前を先にプレヤーリストに入れる
		ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		APlayerState* PS = GetWorld()->GetFirstPlayerController<APlayerState>();
		if (GI)
		{
			UE_LOG(LogTemp, Warning, TEXT("Server adds it's own name in the list"));
			AddPlayerToList(PS, GI->GIPlayerName);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Not the server. Cannot add it's own name to list"));
	}
}

void AGS_MenuGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// レプリケーションするプロパティを追加
	DOREPLIFETIME(AGS_MenuGameState, GSSessionName);
	DOREPLIFETIME(AGS_MenuGameState, GSReadyPlayers);
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

void AGS_MenuGameState::GetDataFromServer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Server Send Session Name"));
	SetSessionName(GSSessionName);

	//Gets player list from server
	for (auto& list : GSPlayerList) {
		AddPlayerToList(list.Key, list.Value);
		UE_LOG(LogTemp, Warning, TEXT("Client added a name in the list"));
		break;
	}
	//Client adds it's own name in the list
	ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	APlayerState* PS = GetWorld()->GetFirstPlayerController<APlayerState>();
	if (GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server adds it's own name in the list"));
		AddPlayerToList(PS, GI->GIPlayerName);
	}
}

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


/// <summary>
/// プレヤーのリストの管理。各プレヤーのID（APlayerState）と名前を記入する。
/// </summary>

void AGS_MenuGameState::AddPlayerToList_Implementation(APlayerState* PS, const FString& PName)
{
	if (PS) {
		GSPlayerList.Add(PS, PName);
		UE_LOG(LogTemp, Warning, TEXT("AddPlayerToList() executed"));
	}
}

void AGS_MenuGameState::RemovePlayerFromList_Implementation(APlayerState* PS)
{
	UE_LOG(LogTemp, Warning, TEXT("RemovePlayerFromList() executed"));
	for (auto& list : GSPlayerList) {
		if (list.Key == PS) {
			GSPlayerList.Remove(list.Key);  //リストからプレヤーを取り消す
			UE_LOG(LogTemp, Warning, TEXT("A player has been removed"));
			break;
		}
	}
}

void AGS_MenuGameState::HostAddReadyPlayer_Implementation()
{
	GSReadyPlayers += 1;
}

void AGS_MenuGameState::HostRemoveReadyPlayer_Implementation()
{
	GSReadyPlayers -=1 ;
}

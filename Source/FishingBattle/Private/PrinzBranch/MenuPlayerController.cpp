// Fill out your copyright notice in the Description page of Project Settings.

#include "PrinzBranch/MenuPlayerController.h"
#include "PrinzBranch/GS_MenuGameState.h"
#include "PrinzBranch/LANGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"

#pragma region Initialize
void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// client
	//if (!HasAuthority())
	//{
	//if (HasAuthority())			//ONLY SERVER
	//{
	//	ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//	if (GI) {
	//		UE_LOG(LogTemp, Warning, TEXT("Client Ask Server Data"));
	//	//	GetDataFromServer(GI->GIPlayerName);
	//		AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
	//		if (GS) {
	//			GS->AddPlayerToList(PlayerState, GS->GSPlayerName);
	//		}
	//	}

		//UE_LOG(LogTemp, Warning, TEXT("Client Send Server Data"));
		//SendDataToServer();
	//}
	/*}*/
}
#pragma endregion

#pragma region Send and Get Data to/from Server
//void AMenuPlayerController::GetDataFromServer_Implementation(const FString& InName)
//{
//	if (!HasAuthority()) {
//		UE_LOG(LogTemp, Warning, TEXT("Server Send Data"));
//		AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
//		if (GS) {
//			GS->GetDataFromServer();
//			GS->AddPlayerToList(PlayerState, InName);
//		}
//	}
//}

//void AMenuPlayerController::SendDataToServer_Implementation(const FString& InName)
//{
//	if (HasAuthority()) {
//		UE_LOG(LogTemp, Warning, TEXT("Client Send Data"));
//		//	ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//		AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
//		APlayerState* PS = GetWorld()->GetFirstPlayerController<APlayerState>();
//		GS->AddPlayerToList_Implementation(PS, InName); //Serverのプレヤーリストに自身を追加する
//	}
//}


//void AMenuPlayerController::SendReadyToServer_Implementation(bool isReady)
//{
//	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
//	if (isReady) {
//		GS->HostAddReadyPlayer_Implementation();
//	}
//	else {
//		GS->HostRemoveReadyPlayer_Implementation();
//	}
//}
//
//void AMenuPlayerController::SendLogoutToServer_Implementation(APlayerState* Exiting)
//{
//	UE_LOG(LogTemp, Warning, TEXT("Client Notify Logout"));
//	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
//	//APlayerState* PS = GetWorld()->GetFirstPlayerController<APlayerState>();
//	GS->RemovePlayerFromList_Implementation(Exiting); //Serverのプレヤーリストに自身を追加する
//}

#pragma endregion


/// <summary>
/// ログアウトした途端にサーバーに知らせる
/// </summary>
/// <param name="Exiting"></param>
//void AMenuPlayerController::Logout(AController* Exiting)
//{
//	Super::Logout(Exiting);
//
//	APlayerState* PS = GetWorld()->GetFirstPlayerController<APlayerState>();
//	//AMenuPlayerController* PC = Cast<AMenuPlayerController>(Exiting);
//	if (IsLocalController())
//	{
//		SendLogoutToServer(PS);
//	}
//}
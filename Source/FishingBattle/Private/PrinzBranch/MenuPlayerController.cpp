// Fill out your copyright notice in the Description page of Project Settings.

#include "PrinzBranch/MenuPlayerController.h"
#include "PrinzBranch/GS_MenuGameState.h"
#include "PrinzBranch/LANGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AMenuPlayerController::GetDataFromServer_Implementation(const FString& InName)  //Server Only
{
	UE_LOG(LogTemp, Warning, TEXT("Server Send Data"));
	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
	if (GS) {
		GS->SendData();
		GS->AddPlayerToList(PlayerState, InName);
	}
}

void AMenuPlayerController::SendDataToServer_Implementation(const FString& InName) //Client only
{
	UE_LOG(LogTemp, Warning, TEXT("Client Send Data"));
//	ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
	APlayerState* PS = GetWorld()->GetFirstPlayerController<APlayerState>();
		GS->AddPlayerToList_Implementation(PS, InName); //Serverのプレヤーリストに自身を追加する
		//Get ID in return ?
}

void AMenuPlayerController::SendLogoutToServer_Implementation(APlayerState* Exiting)
{
	UE_LOG(LogTemp, Warning, TEXT("Client Notify Logout"));
	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
	//APlayerState* PS = GetWorld()->GetFirstPlayerController<APlayerState>();
	GS->RemovePlayerFromList_Implementation(Exiting); //Serverのプレヤーリストに自身を追加する
}

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// client
	//if (!HasAuthority())
	//{
		if (IsLocalController())
		{
			ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (GI) {
				UE_LOG(LogTemp, Warning, TEXT("Client Ask Server Data"));
				GetDataFromServer(GI->GIPlayerName);
			}
	
			//UE_LOG(LogTemp, Warning, TEXT("Client Send Server Data"));
			//SendDataToServer();
		}
	/*}*/
}
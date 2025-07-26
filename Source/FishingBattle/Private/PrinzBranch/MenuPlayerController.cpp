// Fill out your copyright notice in the Description page of Project Settings.

#include "PrinzBranch/MenuPlayerController.h"
#include "PrinzBranch/GS_MenuGameState.h"
#include "PrinzBranch/LANGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AMenuPlayerController::GetDataFromServer_Implementation()  //Server Only
{
	UE_LOG(LogTemp, Warning, TEXT("Server Send Data"));
	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
	if (GS) {
		GS->SendData();
	}
}

void AMenuPlayerController::SendDataToServer_Implementation() //Client only
{
	UE_LOG(LogTemp, Warning, TEXT("Client Send Data"));
	//ULANGameInstance* GI = UGameplayStatics::GetGameInstance(this)->GetSubsystem<ULANGameInstance>(); //GameInstanceにあるClientの自身の名前を取得するため
	ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
	if (GI && GS) {
		GS->AddPlayerToList_Implementation(this, GI->GIPlayerName); //Serverのプレヤーリストに自身を追加する
		//Get ID in return ?
	}
}

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// client
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client Ask Server Data"));
		GetDataFromServer();

		UE_LOG(LogTemp, Warning, TEXT("Client Send Server Data"));
		SendDataToServer();
	}
}
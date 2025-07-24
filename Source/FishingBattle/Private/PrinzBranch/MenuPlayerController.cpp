// Fill out your copyright notice in the Description page of Project Settings.

#include "PrinzBranch/MenuPlayerController.h"
#include "PrinzBranch/GS_MenuGameState.h"

void AMenuPlayerController::GetDataFromServer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Server Send Data"));
	AGS_MenuGameState* GS = GetWorld()->GetGameState<AGS_MenuGameState>();
	if (GS) {
		GS->SendData();
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
	}
}
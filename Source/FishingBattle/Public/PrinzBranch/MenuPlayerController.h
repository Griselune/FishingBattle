// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
//#include "GameFramework/GameModeBase.h"
//#include "GameFramework/GameMode.h"
//#include "PrinzBranch/GM_MenuGameMode.h"

#include "MenuPlayerController.generated.h"
UCLASS()
class FISHINGBATTLE_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Server, Reliable)
	void GetDataFromServer(const FString& InName);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SendDataToServer(const FString& InName);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SendLogoutToServer(APlayerState* Exiting);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SendReadyToServer(bool isReady);

protected:

	virtual void BeginPlay() override;

	//virtual void Logout(AController* Exiting) override;
	
};

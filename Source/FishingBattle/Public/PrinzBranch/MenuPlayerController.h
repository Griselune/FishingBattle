// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PrinzBranch/MatchMakingWidget.h"
//#include "GameFramework/GameMode.h"
//#include "PrinzBranch/GM_MenuGameMode.h"

#include "MenuPlayerController.generated.h"
UCLASS()
class FISHINGBATTLE_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	UMatchMakingWidget* MatchMakingWidget;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SetPlayerReady(bool IsPlayerReady);

	UFUNCTION()
	void SessionDataInitialize();

protected:

	virtual void BeginPlay() override;

	//virtual void Logout(AController* Exiting) override;
	
};

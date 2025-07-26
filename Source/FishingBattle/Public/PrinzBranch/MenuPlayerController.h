// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Server, Reliable)
	void GetDataFromServer();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void SendDataToServer();

protected:

	virtual void BeginPlay() override;

};

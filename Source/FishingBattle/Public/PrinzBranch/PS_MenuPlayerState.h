// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"


#include "OnlineSubsystem.h" // Needed for FUniqueNetId
#include "Interfaces/OnlineIdentityInterface.h" // Needed for GetUniqueNetId
#include "PS_MenuPlayerState.generated.h"
UCLASS()
class FISHINGBATTLE_API APS_MenuPlayerState : public APlayerState
{
	GENERATED_BODY()
	
private:
	//virtual void SetPlayerName(const FString& S);
};

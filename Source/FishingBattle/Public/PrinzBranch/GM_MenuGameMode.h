// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerController.h"
 

#include "GM_MenuGameMode.generated.h"
UCLASS()
class FISHINGBATTLE_API AGM_MenuGameMode : public AGameMode
{
	GENERATED_BODY()
	

protected:

	virtual void BeginPlay() override;
public:
	virtual void OnPostLogin(AController* NewPlayer)override;
	virtual void Logout(AController* Exiting) override;

};

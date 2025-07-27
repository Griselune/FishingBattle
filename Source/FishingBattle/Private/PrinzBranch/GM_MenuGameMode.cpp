// Fill out your copyright notice in the Description page of Project Settings.

#include "PrinzBranch/GM_MenuGameMode.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "PrinzBranch/MenuPlayerController.h"
#include "PrinzBranch/LANGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void AGM_MenuGameMode::OnPostLogin(AController* NewPlayer)
{
    Super::OnPostLogin(NewPlayer);

    AMenuPlayerController* PC = Cast<AMenuPlayerController>(NewPlayer);
    if (PC && PC->IsLocalController())
    {
        ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        PC->SendDataToServer(GI->GIPlayerName);
    }

}

//void AGM_MenuGameMode::Logout(AController* Exiting)
//{
//    Super::Logout(Exiting);
//
//    AMenuPlayerController* PC = Cast<AMenuPlayerController>(Exiting);
//    if (PC && PC->IsLocalController())
//    {
//        ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//        PC->SendLogoutToServer(Exiting);
//    }
//}
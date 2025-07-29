// Fill out your copyright notice in the Description page of Project Settings.

#include "PrinzBranch/GM_MenuGameMode.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "PrinzBranch/MenuPlayerController.h"
#include "PrinzBranch/LANGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

/// <summary>
/// ログインした途端にサーバーに情報を送る
/// </summary>
/// <param name="NewPlayer"></param>
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

/// <summary>
/// ログアウトした途端にサーバーに知らせる
/// </summary>
/// <param name="Exiting"></param>
void AGM_MenuGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    APlayerState* PS = GetWorld()->GetFirstPlayerController<APlayerState>();
    AMenuPlayerController* PC = Cast<AMenuPlayerController>(Exiting);
    if (PC->IsLocalController())
    {
        //ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        PC->SendLogoutToServer(PS);
    }
}
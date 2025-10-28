// Fill out your copyright notice in the Description page of Project Settings.

#include "PrinzBranch/GM_MenuGameMode.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "PrinzBranch/MenuPlayerController.h"
#include "PrinzBranch/LANGameInstance.h"
#include "PrinzBranch/GS_MenuGameState.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PrinzBranch/PS_MenuPlayerState.h"

void AGM_MenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    //ビルドバグ対策
    ULANGameInstance* GI = Cast<ULANGameInstance>(GetGameInstance());
    AGS_MenuGameState* GS = GetGameState<AGS_MenuGameState>();
    if (GI && GS)
    {
        GS->GSSessionName = GI->GISessionName;
        GS->GSSessionPlayerLimit = GI->GISessionPlayerLimit;
        GS->GSSessionTimeLimit = GI->GISessionTimeLimit;
        GS->GSisDeathMatch = GI->GIisDeathMatch;
        GS->GSisBattleRoyale = GI->GIisBattleRoyale;
    }
    // ---- Force update for listen server’s local player ----
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AMenuPlayerController* PC = Cast<AMenuPlayerController>(It->Get());
        if (PC && PC->IsLocalController())  // This is the host’s client side
        {
            PC->SessionDataInitialize();  // Manually refresh UI now that data is valid
        }
    }

   /* if (HasAuthority())
    {
        for (APlayerState* PS : GameState->PlayerArray)
        {
            APS_MenuPlayerState* MyPS = Cast<APS_MenuPlayerState>(PS);
            AGS_MenuGameState* GS = GetGameState<AGS_MenuGameState>();
            if (MyPS)
            {
                GS->GSPlayerList.Add(MyPS, MyPS->PSPlayerName);
            }
        }
    }*/
}



/// <summary>
/// ログインした途端にサーバーに情報を送る
/// </summary>
/// <param name="NewPlayer"></param>
void AGM_MenuGameMode::OnPostLogin(AController* NewPlayer)
{
    Super::OnPostLogin(NewPlayer);

    //APlayerState* PS = NewPlayer->PlayerState;
    //AGS_MenuGameState* GS = GetGameState<AGS_MenuGameState>();

    //if (GS && PS)
    //{
    //    GS->AddPlayerToList(PS);
    //}

    ////get server's controller and gamestate
    //APlayerController* LocalPlayerController = UGameplayStatics::GetPlayerController(this, 0);
    //AGS_MenuGameState* GSserver = Cast<AGS_MenuGameState>(LocalPlayerController);
    //
    //

    ////get connected client's gamestate
    //AGS_MenuGameState* GSclient = Cast<AGS_MenuGameState>(NewPlayer);
  
    ////Server adds connected client's name to all lists
  
    //ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    //APlayerState* PSserver = GetWorld()->GetFirstPlayerController<APlayerState>();
    //if (PSserver && NewPlayer->HasAuthority()) {
    //    GSserver->AddPlayerToList(PSserver, "TestServer" /*GI->GIPlayerName*/);
    //   // return;
    //}

    //APlayerState* PSclient = NewPlayer->GetPlayerState<APlayerState>();
    //if (PSclient && !NewPlayer->HasAuthority()) {
    //   // GSserver->GetDataFromServer(NewPlayer);
    //    GSserver->AddPlayerToList(PSclient, "testClient");
    //  //  return;
    //}
    
   
    //TEST//


    //if (!HasAuthority()) {
    //    ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    //    APlayerState* PSclient = NewPlayer->GetPlayerState<APlayerState>();
    //    //server gives player list to connected client
    //    GI->GIPlayerList.Append(GSserver->GSPlayerList);
    //    GSclient->GetDataFromServer();
    //}


    //idk
    //AMenuPlayerController* PCclient = Cast<AMenuPlayerController>(NewPlayer);
    //if (PCclient /*&& PC->IsLocalController()*/)
    //{
    //    ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    //    
    //    //PC->SendDataToServer(GI->GIPlayerName);
    //}

}

/// <summary>
/// ログアウトした途端にサーバーに知らせる
/// </summary>
/// <param name="Exiting"></param>
void AGM_MenuGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    //APlayerState* PS = GetWorld()->GetFirstPlayerController<APlayerState>();
    //AMenuPlayerController* PC = Cast<AMenuPlayerController>(Exiting);
    //if (PC->IsLocalController())
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("///// EXITING /////"));
    //    //ULANGameInstance* GI = Cast<ULANGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    //    PC->SendLogoutToServer(PS);
    //}
}
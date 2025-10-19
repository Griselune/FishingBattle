// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PS_MenuPlayerState.generated.h"




UCLASS()
class FISHINGBATTLE_API APS_MenuPlayerState : public APlayerState
{
    GENERATED_BODY()


protected:
    virtual void BeginPlay() override;

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

    UFUNCTION()
    void OnRep_PName();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Name", Replicated, ReplicatedUsing = OnRep_PName)
    FString PSPlayerName;

    //  UPROPERTY(BlueprintReadWrite, EditAnywhere)
     // TMap<APlayerState*, FString> PSPlayerList;


      /// <summary>
      /// Functions
      /// </summary>
    //UFUNCTION(BlueprintCallable, Server, Reliable)
    //void GetDataFromServer(AController* NewPlayer);

    UFUNCTION(BlueprintCallable, Server, Reliable)
    void Server_SetPlayerName(const FString& NewName);

    //UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
    //void AddPlayerToList(APlayerState* PS, const FString& PName);

    //UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
    //void RemovePlayerFromList(APlayerState* PS);

    //UFUNCTION(BlueprintCallable, Server, Reliable)
    //void HostAddReadyPlayer();

    //UFUNCTION(BlueprintCallable, Server, Reliable)
    //void HostRemoveReadyPlayer();

};

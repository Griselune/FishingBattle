// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GS_MenuGameState.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API AGS_MenuGameState : public AGameState
{
	GENERATED_BODY()
	
protected:

    virtual void BeginPlay();

public:

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

    UFUNCTION()
    void OnRep_SessionData();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data", Replicated, ReplicatedUsing = OnRep_SessionData)
    FString GSSessionName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data", Replicated, ReplicatedUsing = OnRep_SessionData)
    FString GSSessionPassword;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data", Replicated, ReplicatedUsing = OnRep_SessionData)
    int32 GSSessionPlayerLimit = 2;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data", Replicated, ReplicatedUsing = OnRep_SessionData)
    FString GSPlayerName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data", Replicated, ReplicatedUsing = OnRep_SessionData)
    int32 GSSessionTimeLimit = 3;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data", Replicated, ReplicatedUsing = OnRep_SessionData)
    bool GSisDeathMatch = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data", Replicated, ReplicatedUsing = OnRep_SessionData)
    bool GSisBattleRoyale = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shared Data")
    TMap<APlayerState*, FString> GSPlayerList;


    /// <summary>
    /// Functions
    /// </summary>
    void SendData();

    UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
    void AddPlayerToList(APlayerState* PS, const FString& PName);

    UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
    void RemovePlayerFromList(APlayerState* PS);


    UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
    void HostAddPlayerList();

private:

    //UFUNCTION(Server, Reliable)
    //void GetDataFromServer();

    UFUNCTION(NetMulticast, Reliable)
    void SetSessionName(const FString& Name);

    UFUNCTION(NetMulticast, Reliable)
    void SetSessionPassword(const FString& Password);

    UFUNCTION(NetMulticast, Reliable)
    void SetSessionPlayerLimit(const int32& PlayerLimit);

    UFUNCTION(NetMulticast, Reliable)
    void SetPlayerName(const FString& PlayerName);  //name

    UFUNCTION(NetMulticast, Reliable)
    void SetSessionTimeLimit(const int32& TimeLimit);

    UFUNCTION(NetMulticast, Reliable)
    void SetSessionIsDeathMatch(bool isDM);

    UFUNCTION(NetMulticast, Reliable)
    void SetSessionIsBattleRoyale(bool isBR);



};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "InventoryWeapon.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState_T.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API APlayerState_T : public APlayerState
{
	GENERATED_BODY()

public:

	static const int slotMax = 4;

	UPROPERTY(Replicated,ReplicatedUsing = OnRep_Inventory)
	TArray<FInventoryWeapon> inventory;

	//UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddWeapon(FName WeaponID); //クライアント用

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_AddWeapon(FName WeaponID);  // サーバー用

	void InventoryInitialize();

	bool HasEmptySlot() const;

    const FInventoryWeapon* GetweaponSlot(int index) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UFUNCTION()
	void OnRep_Inventory();
	
};

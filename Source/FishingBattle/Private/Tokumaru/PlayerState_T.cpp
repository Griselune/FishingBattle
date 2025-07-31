// Fill out your copyright notice in the Description page of Project Settings.


#include "Tokumaru/PlayerState_T.h"
#include "Net/UnrealNetwork.h"

void APlayerState_T::Server_AddWeapon(TSubclassOf<AActor> WeaponID)
{
	//Multi_AddWeapon(WeaponID);
	if (!HasEmptySlot()) {
		UE_LOG(LogTemp, Error, TEXT("cantAddWeapon!"));
		return;
	}


	FInventoryWeapon newWeapon;
	newWeapon.weaponActor = WeaponID;
	inventory.Add(newWeapon);
	//OnRep_Inventory();

}

void APlayerState_T::InventoryInitialize()
{
	inventory.Empty();
	OnRep_Inventory();
}

bool APlayerState_T::HasEmptySlot() const
{
	return inventory.Num() < slotMax;
}

const FInventoryWeapon* APlayerState_T::GetweaponSlot(int index) const
{
	return inventory.IsValidIndex(index) ? &inventory[index] : nullptr;
}



void APlayerState_T::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerState_T, inventory);
	DOREPLIFETIME(APlayerState_T, InGamePlay);
	DOREPLIFETIME(APlayerState_T, DeadCounter);

}

void APlayerState_T::OnRep_Inventory()
{
	UE_LOG(LogTemp, Log, TEXT("インベントリ更新"));
}

void APlayerState_T::OnRep_InGamePlay()
{
	UE_LOG(LogTemp, Log, TEXT("スタンバイok"));
}


void APlayerState_T::OnRep_DeadCounter()
{
	UE_LOG(LogTemp, Error, TEXT("AddDeadCounter"));
}

void APlayerState_T::SetName_Implementation(const FString& PlayerName)
{
	Name = PlayerName;
}

FString APlayerState_T::GetName() const
{
	return Name;
}
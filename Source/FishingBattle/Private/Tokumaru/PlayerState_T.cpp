// Fill out your copyright notice in the Description page of Project Settings.


#include "Tokumaru/PlayerState_T.h"
#include "Net/UnrealNetwork.h"



void APlayerState_T::Server_AddWeapon(FName WeaponID)
{
	//Multi_AddWeapon(WeaponID);
	if (!HasEmptySlot()) {
		UE_LOG(LogTemp, Error, TEXT("cantAddWeapon!"));
		return;
	}

	FInventoryWeapon newWeapon;
	newWeapon.weaponName = WeaponID;
	inventory.Add(newWeapon);
	OnRep_Inventory();

}

void APlayerState_T::Multi_AddWeapon_Implementation(FName WeaponID)
{
	if (!HasEmptySlot()) {
		UE_LOG(LogTemp, Error, TEXT("cantAddWeapon!"));
		return;
	}

	FInventoryWeapon newWeapon;
	newWeapon.weaponName = WeaponID;
	inventory.Add(newWeapon);
	OnRep_Inventory();

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

}

void APlayerState_T::OnRep_Inventory()
{
	UE_LOG(LogTemp, Log, TEXT("インベントリ更新"));
}

//bool APlayerState_T::Server_AddWeapon_Validate(FName WeapontID) {
//	return true;
//}

bool APlayerState_T::Multi_AddWeapon_Validate(FName WeapontID) {
	return true;
}


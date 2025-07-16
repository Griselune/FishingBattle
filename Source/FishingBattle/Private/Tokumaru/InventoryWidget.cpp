// Fill out your copyright notice in the Description page of Project Settings.


#include "Tokumaru/InventoryWidget.h"

FName UInventoryWidget::SetFishRod(FName weaponID)
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			if (ps->inventory.Num() >= 1) {
				return ps->inventory[0].weaponName;
			}
		}
	}
	return FName("NoWeapon");
}

FName UInventoryWidget::SetInventory1(FName weaponID)
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			if (ps->inventory.Num() >= 2) {
				return ps->inventory[1].weaponName;
			}
		}
	}
	return FName("NoWeapon");
}

FName UInventoryWidget::SetInventory2(FName weaponID)
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			if (ps->inventory.Num() >= 3) {
				return ps->inventory[2].weaponName;
			}
		}
	}
	return FName("NoWeapon");
}

FName UInventoryWidget::SetInventory3(FName weaponID)
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			if (ps->inventory.Num() >= 4) {
				return ps->inventory[3].weaponName;
			}
		}
	}
	return FName("NoWeapon");
}

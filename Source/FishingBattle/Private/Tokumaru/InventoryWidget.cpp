// Fill out your copyright notice in the Description page of Project Settings.


#include "Tokumaru/InventoryWidget.h"

FName UInventoryWidget::SetFishRod()
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			if (ps->inventory.Num() >= 1) {
				return FName("fishrod");
			}
		}
	}
	return FName("NoWeapon");
}

FName UInventoryWidget::SetInventory1()
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			if (ps->inventory.Num() >= 2) {
				return FName("アクターある");
			}
		}
	}
	return FName("NoWeapon");
}

FName UInventoryWidget::SetInventory2()
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			if (ps->inventory.Num() >= 3) {
				return FName("アクターある");
			}
		}
	}
	return FName("NoWeapon");
}

FName UInventoryWidget::SetInventory3()
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			if (ps->inventory.Num() >= 4) {
				return FName("アクターある");
			}
		}
	}
	return FName("NoWeapon");
}

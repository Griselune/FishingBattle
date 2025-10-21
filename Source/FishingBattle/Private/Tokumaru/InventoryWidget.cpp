// Fill out your copyright notice in the Description page of Project Settings.


#include "Tokumaru/InventoryWidget.h"
UTexture2D* UInventoryWidget::SetInventory(int number)
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			if (ps->inventory.Num() >= number + 1) {
				if (UClass* weaponClass = ps->inventory[number].weaponActor) {
					if (ACPPBaseWeapon* baseWeapon = Cast<ACPPBaseWeapon>(weaponClass->GetDefaultObject())) {
						return baseWeapon->Icon;
						//switch (weaponName) {
						//case ECPPWeaponType::WeaponTest1:
						//	return gourdfishTexturer;
						//	break;
						//case ECPPWeaponType::WeaponTest2:
						//	return coelacanthTexturer;
						//	break;
						//case ECPPWeaponType::WeaponTest3:
						//	return sharkTexturer;
						//	break;
						//default:
						//	return non;
						//}
					}

				}

			}
		}
	}
	return non;
}

bool UInventoryWidget::CanRollState()
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			return ps->CanRollInPS;
		}
	}
	return false;
}

int UInventoryWidget::GetWeaponArrayIndex()
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			return ps->inventory.Num();
		}
	}
	return 0;
}

void UInventoryWidget::WidgetBegin()
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			ps->OnInventoryUpdated.AddDynamic(this,&UInventoryWidget::SetTextureUpdate);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Tokumaru/InventoryWidget.h"

UTexture2D* UInventoryWidget::SetInventory1()
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			if (ps->inventory.Num() >= 2) {
				if (UClass* weaponClass = ps->inventory[1].weaponActor) {
					if (ACPPBaseWeapon* baseWeapon = Cast<ACPPBaseWeapon>(weaponClass->GetDefaultObject())) {
						ECPPWeaponType weaponName = baseWeapon->WeaponType;
						switch (weaponName) {
						case ECPPWeaponType::WeaponTest1:
							return gourdfishTexturer;
							break;
						case ECPPWeaponType::WeaponTest2:
							return coelacanthTexturer;
							break;
						case ECPPWeaponType::WeaponTest3:
							return sharkTexturer;
							break;
						default:
							return non;
						}
					}

				}

			}
		}
	}
	return non;
}

UTexture2D* UInventoryWidget::SetInventory2()
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			if (ps->inventory.Num() >= 3) {
				if (UClass* weaponClass = ps->inventory[2].weaponActor) {
					if (ACPPBaseWeapon* baseWeapon = Cast<ACPPBaseWeapon>(weaponClass->GetDefaultObject())) {
						ECPPWeaponType weaponName = baseWeapon->WeaponType;
						switch (weaponName) {
						case ECPPWeaponType::WeaponTest1:
							return gourdfishTexturer;
							break;
						case ECPPWeaponType::WeaponTest2:
							return coelacanthTexturer;
							break;
						case ECPPWeaponType::WeaponTest3:
							return sharkTexturer;
							break;
						default:
							return non;
						}
					}

				}

			}
		}
	}
	return non;
}

UTexture2D* UInventoryWidget::SetInventory3()
{
	APlayerController* pc = GetOwningPlayer();
	if (pc) {
		APlayerState_T* ps = pc->GetPlayerState<APlayerState_T>();
		if (ps) {
			if (ps->inventory.Num() >= 4) {
				if (UClass* weaponClass = ps->inventory[3].weaponActor) {
					if (ACPPBaseWeapon* baseWeapon = Cast<ACPPBaseWeapon>(weaponClass->GetDefaultObject())) {
						ECPPWeaponType weaponName = baseWeapon->WeaponType;
						switch (weaponName) {
						case ECPPWeaponType::WeaponTest1:
							return gourdfishTexturer;
							break;
						case ECPPWeaponType::WeaponTest2:
							return coelacanthTexturer;
							break;
						case ECPPWeaponType::WeaponTest3:
							return sharkTexturer;
							break;
						default:
							return non;
						}
					}

				}

			}
		}
	}
	return non;
}

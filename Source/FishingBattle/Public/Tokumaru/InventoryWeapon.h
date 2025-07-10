// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWeapon.generated.h"

USTRUCT(BlueprintType)
struct FInventoryWeapon {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName weaponName;

};
/**
 * 
 */
//class FISHINGBATTLE_API InventoryWeapon
//{
//public:
//	InventoryWeapon();
//	~InventoryWeapon();
//};

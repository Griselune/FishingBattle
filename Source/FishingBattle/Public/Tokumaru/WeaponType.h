// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    None UMETA(DisplayName = "None"),
    FishRod UMETA(DisplayName = "FishRod"),
    LightWeapon UMETA(DisplayName = "LightWeapon"),
    HeavyWeapon UMETA(DisplayName = "HeavyWeapon"),
};

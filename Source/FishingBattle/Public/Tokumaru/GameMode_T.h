// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameMode_T.generated.h"

/**
 *
 */
UCLASS()
class AGameMode_T : public AGameMode
{
	GENERATED_BODY()
public:
	AGameMode_T();

	/// <summary>
    /// リスポーン要求
    /// </summary>
    /// <param name="Controller"></param>
	void RespawnPlayerT(AController* Controller);

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TMap<FName, TSubclassOf<AActor>> weaponMap;

	TSubclassOf<AActor> GetWeaponClass(FName WeaponID) const;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TakimotoBranch/CPPBaseWeapon.h"
#include "CPPWeaponTest3.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API ACPPWeaponTest3 : public ACPPBaseWeapon
{
	GENERATED_BODY()
	
public:

	virtual void Attack_Implementation() override;

	virtual void OnHit_Implementation(AActor* HitActor) override;
};

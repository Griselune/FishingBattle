// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TakimotoBranch/CPPBaseWeapon.h"
#include "CPPWeaponTest2.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API ACPPWeaponTest2 : public ACPPBaseWeapon
{
	GENERATED_BODY()

public:

	virtual void Attack_Implementation() override;

	virtual void OnHit_Implementation(AActor* HitActor) override;

	virtual void Attack_Begin_Implementation() override;

	virtual void Attack_Finish_Implementation() override;
};

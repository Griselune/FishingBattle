// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TakimotoBranch/CPPBaseWeapon.h"
#include "CPPWeaponTest1.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API ACPPWeaponTest1 : public ACPPBaseWeapon
{
	GENERATED_BODY()
	
protected:

	virtual void Tick(float DeltaSeconds) override;

public:

	virtual void Attack_Implementation() override;

	virtual void OnHit_Implementation(AActor* HitActor) override;

	virtual void Attack_Begin_Implementation() override;

	virtual void Attack_Finish_Implementation() override;
};

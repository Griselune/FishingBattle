// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TakimotoBranch/CPPBaseWeapon.h"
#include "CPPFishingRod.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API ACPPFishingRod : public ACPPBaseWeapon
{
	GENERATED_BODY()
	
protected:

	virtual void Tick(float DeltaSeconds);

public:

	virtual void Attack_Implementation() override;

	virtual void OnHit_Implementation(AActor* HitActor) override;

	virtual void BeginPlay() override;

};

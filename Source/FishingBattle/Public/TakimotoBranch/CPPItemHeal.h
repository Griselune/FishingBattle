// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TakimotoBranch/CPPBaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "CPPItemHeal.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API ACPPItemHeal : public ACPPBaseItem
{
	GENERATED_BODY()

private:

	bool IsHeal;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HealSound;
	
protected:

	UFUNCTION(BlueprintCallable, Category = "Item")
	void Heal(AActor* Player);
};

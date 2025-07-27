// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TakimotoBranch/CPPBaseItem.h"
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

	UFUNCTION(BlueprintCallable, Category = "Item")
	void Heal(AActor* Player);
};

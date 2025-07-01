// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IPUtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API UIPUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Network")
	static FString GetLocalIPAddress();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HPUpdaterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHPUpdaterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FISHINGBATTLE_API IHPUpdaterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/// <summary>
	/// HPを更新
	/// </summary>
	/// <param name="Max">最大HP</param>
	/// <param name="NewHP">新しいHP</param>
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHP(float Max, float NewHP);
};

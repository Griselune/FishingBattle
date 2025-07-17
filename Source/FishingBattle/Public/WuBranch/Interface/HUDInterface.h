// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FISHINGBATTLE_API IHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/// <summary>
	/// ゲーム終了時のUIを表示
	/// </summary>
	/// <param name="bShow">true: 勝利, false: 負け</param>
	UFUNCTION(BlueprintImplementableEvent)
	void ShowFinishUI(bool bShow);
};

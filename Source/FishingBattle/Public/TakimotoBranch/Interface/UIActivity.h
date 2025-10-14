// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UIActivity.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUIActivity : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FISHINGBATTLE_API IUIActivity
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/// <summary>
	/// UIを表示
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent)
	void ShowUI();

	/// <summary>
	/// UIを非表示
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent)
	void CloseUI();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NameUI.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNameUI : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FISHINGBATTLE_API INameUI
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/// <summary>
	/// 名前を設定
	/// </summary>
	/// <param name="Name">名前</param>
	UFUNCTION(BlueprintImplementableEvent)
	void SetName(const FString& Name);

	/// <summary>
	/// 名前を隠す
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent)
	void HideName();

	/// <summary>
	/// 名前を表示
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent)
	void ShowName();
};

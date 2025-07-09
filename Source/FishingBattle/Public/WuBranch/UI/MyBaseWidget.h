// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyBaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API UMyBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
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

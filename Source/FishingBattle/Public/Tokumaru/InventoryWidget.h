// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tokumaru/PlayerState_T.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:


	UFUNCTION(BlueprintCallable)
	FName SetFishRod();

	/// <summary>
	/// インベントリ表示UIの1番目変更
	/// </summary>
	/// <param name="weaponID"></param>
	UFUNCTION(BlueprintCallable)
	FName SetInventory1();

	UFUNCTION(BlueprintCallable)
	FName SetInventory2();

	UFUNCTION(BlueprintCallable)
	FName SetInventory3();

	//インベントリの配列がアクターの実体になったので、武器タイプを参照してテキスト（イメージ）を変えるようにすること
	//得丸
};

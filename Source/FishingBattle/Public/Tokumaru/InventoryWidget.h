// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tokumaru/PlayerState_T.h"
#include "TakimotoBranch/CPPBaseWeapon.h"
#include "TakimotoBranch/CPPWeaponType.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	/// <summary>
	/// インベントリ表示UIの1番目変更
	/// </summary>
	/// <param name="weaponID"></param>
	UFUNCTION(BlueprintCallable)
	UTexture2D* SetInventory1();

	UFUNCTION(BlueprintCallable)
	UTexture2D* SetInventory2();

	UFUNCTION(BlueprintCallable)
	UTexture2D* SetInventory3();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture2D* sharkTexturer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture2D* gourdfishTexturer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture2D* coelacanthTexturer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture2D* non;

	//インベントリの配列がアクターの実体になったので、武器タイプを参照してテキスト（イメージ）を変えるようにすること
	//得丸
};

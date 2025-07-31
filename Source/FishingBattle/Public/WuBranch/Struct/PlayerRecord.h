// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerRecord.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerRecord
{
	GENERATED_BODY()

	/// <summary>
	/// プレレイヤーID
	/// </summary>
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Player Record")
	int32 ID;

	/// <summary>
	/// 死亡回数
	/// </summary>
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Player Record")
	int DeathCount;

	/// <summary>
	/// 与えたダメージの合計
	/// </summary>
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Player Record")
	float TotalDamage;

	/// <summary>
	/// 名前
	/// </summary>
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Player Record")
	FString Name;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStateList.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EGameStateList : uint8
{
	Init UMETA(DisplayName = "Init"), // 初期化
	CheckPlayerState UMETA(DisplayName = "CheckPlayerState"), // プレイヤー状態の確認
	BeforeStart UMETA(DisplayName = "BeforeStart"), // 開始前
	Started UMETA(DisplayName = "Started"), // 開始
	Finished UMETA(DisplayName = "Finished"), // 終了
	Max UMETA(Hidden) // カウント用
};

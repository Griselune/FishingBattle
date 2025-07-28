// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameMode_T.generated.h"

class APlayerState_T;
struct FPlayerRecord;

/**
 *
 */
UCLASS()
class AGameMode_T : public AGameMode
{
	GENERATED_BODY()
public:
	AGameMode_T();

	/// <summary>
    /// リスポーン要求
    /// </summary>
    /// <param name="Controller"></param>
	void RespawnPlayerT(AController* Controller);

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TMap<FName, TSubclassOf<AActor>> weaponMap;

	TSubclassOf<AActor> GetWeaponClass(FName WeaponID) const;

	// 2025.07.26 ウー start
	virtual void StartPlay() override;

private:

	/// <summary>
	/// ゲーム状態が変わった時
	/// </summary>
	/// <param name="State">新しい状態</param>
	UFUNCTION()
	void OnStateChanged(EGameStateList State);

	/// <summary>
	/// 全プレイヤーのデータを記録する
	/// </summary>
	void AddPlayerDatasToRecord();

	/// <summary>
	/// プレイヤーの記録を作成
	/// </summary>
	/// <param name="Player">プレイヤーのデータ</param>
	/// <returns>プレイヤーの記録</returns>
	FPlayerRecord& MakeRecord(APlayerState_T* Player);

	// 2025.07.26 ウー end
};

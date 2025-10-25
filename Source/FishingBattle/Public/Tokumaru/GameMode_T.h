// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameMode_T.generated.h"

class APlayerState_T;
struct FPlayerRecord;
class AFishingBattleCharacter;

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

	// 2025.10.22 ウー start
#pragma region 勝者の更新
public:

	/// <summary>
	/// 勝者の更新
	/// </summary>
	void UpdateWinner();

	/// <summary>
	/// IDが勝者かどうか
	/// </summary>
	/// <param name="ID">プレイヤーのID</param>
	/// <returns>true: はい、false: いいえ</returns>
	bool IsWinner(int32 ID);

private:

	/// <summary>
	/// 王冠の更新
	/// </summary>
	/// <param name="NewWinner">勝者</param>
	void UpdateCrown(const FPlayerRecord* NewWinner);

	/// <summary>
	///　プレイヤーを見つける
	/// </summary>
	/// <param name="TargetID">対象ID</param>
	AFishingBattleCharacter* FindPlayer(const int32& TargetID);

	/// <summary>
	/// 現在の勝者ID
	/// </summary>
	int32 CurrentWinnerID;
#pragma endregion
	// 2025.10.22 ウー end
};

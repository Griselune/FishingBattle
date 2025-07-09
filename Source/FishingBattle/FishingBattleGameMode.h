// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FishingBattleGameMode.generated.h"

// 2025.07.06 ウー start
class AFishingGround;
// 2025.07.06 ウー end

UCLASS(minimalapi)
class AFishingBattleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFishingBattleGameMode();

	// 2025.07.06 ウー start
	virtual void StartPlay() override;
	// 2025.07.06 ウー end

	/// <summary>
	/// プレイヤーリスポーン
	/// </summary>
	/// <param name="Controller"></param>
	void RespawnPlayer(AController* Controller);
	
private:

	// 2025.07.06 ウー start
	
	/// <summary>
	/// ランダムの位置に釣り場を生成する
	/// </summary>
	void SpawnFishingGroundRandom();

	/// <summary>
	/// 釣り場のサンプル
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fishing", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFishingGround> FishingGroundSample;

	/// <summary>
	/// 一ゲーム内で釣り場の数
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fishing", meta = (AllowPrivateAccess = "true"))
	int FishingGroundNums;

	/// <summary>
	/// 釣り場の生成範囲
	/// </summary>
	TObjectPtr<class AFishingGroundSpawnArea> FishingGroundSpawnArea;

	/// <summary>
	/// 釣り場
	/// </summary>
	TArray<AFishingGround*> Grounds;
	// 2025.07.06 ウー end

};




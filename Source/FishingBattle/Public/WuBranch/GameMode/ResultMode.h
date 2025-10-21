// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ResultMode.generated.h"

class ULANGameInstance;
struct FPlayerRecord;
class AResultPlayer;
class AFirework;

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API AResultMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AResultMode();

	virtual void StartPlay() override;

	/// <summary>
	/// 勝者の名前をゲット
	/// </summary>
	/// <returns>勝者の名前</returns>
	FString GetWinnerName() const;

private:

	/// <summary>
	/// 勝者を探す
	/// </summary>
	void FindWinner();

	/// <summary>
	/// 候補者を見つける
	/// </summary>
	/// <returns>候補者達</returns>
	TArray<FPlayerRecord> FindPotentials();

	/// <summary>
	/// 記録と同じ数のプレイヤーキャラクターを生成
	/// </summary>
	void SpawnPlayer();

	/// <summary>
	/// 新しい生成位置をゲット
	/// </summary>
	/// <param name="PlayerWidth">プレイヤーの幅</param>
	/// <returns>位置</returns>
	FVector GetSpawnLocation(float PlayerWidth);

	/// <summary>
	/// 照明灯の初期化
	/// </summary>
	void InitSpotLight();

	/// <summary>
	/// 勝者を照らしたら
	/// </summary>
	UFUNCTION()
	void OnPointedWinner();

	/// <summary>
	/// 花火を探す
	/// </summary>
	void FindFireworks();

	/// <summary>
	/// 勝者の名前を表示
	/// </summary>
	void ShowWinnerName();

	/// <summary>
	/// プレイヤキャラクタのアニメーション状態を開始
	/// </summary>
	void StartPlayerAnimationState();

	/// <summary>
	/// ゲームインスタンス
	/// </summary>
	ULANGameInstance* MyGameInstance;

	/// <summary>
	/// カメラの位置
	/// </summary>
	FVector CameraLocation;

	/// <summary>
	/// プレイヤー
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AResultPlayer> PlayerClass;

	/// <summary>
	/// 勝者のID
	/// </summary>
	int WinnerID;

	/// <summary>
	/// 勝者の名前
	/// </summary>
	FString WinnerName;

	/// <summary>
	/// キャラクター生成の中心
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector PlayerSpawnCenter;

	/// <summary>
	/// キャラクター生成の範囲(拡張の長さ)
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector PlayerSpawnHalfSize;

	/// <summary>
	/// プレイヤーキャラクター達
	/// </summary>
	TArray<AResultPlayer*> PlayerCharacters;

	/// <summary>
	/// 照明灯
	/// </summary>
	TArray<AActor*> SpotLights;

	/// <summary>
	/// スポットライトの数
	/// </summary>
	int SpotLightNumber;

	/// <summary>
	/// 花火
	/// </summary>
	TArray<AFirework*> Fireworks;

};

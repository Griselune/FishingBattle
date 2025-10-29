// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "InventoryWeapon.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState_T.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);


UCLASS()
class FISHINGBATTLE_API APlayerState_T : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;


	static const int slotMax = 4;

	UPROPERTY(Replicated,ReplicatedUsing = OnRep_Inventory)
	TArray<FInventoryWeapon> inventory;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_DeadCounter, BlueprintReadWrite)
    int DeadCounter = 0;

    UFUNCTION()
    void OnRep_DeadCounter();

	UPROPERTY(BlueprintReadWrite)
	bool CanRollInPS = true;

	UFUNCTION(Server, Reliable)
	void SetName(const FString& PlayerName);

	FString GetName() const;

	//プレイヤーステートクラスからレプリケート関数を動作させる意味がないので、
	// クライアント・サーバーで分ける必要がないが、一応念のため残しておく。
	// 一応レプリケートはできるらしいが、結局処理はプレイヤーからサーバー対応関数の中に
	// 入れてあるのでok
	//UFUNCTION(Server, Reliable, WithValidation)
	
	/// <summary>
	/// プレイヤーステート内のインベントリ配列に武器のアクターを入れる。現在四つまで（釣り竿含む）
	/// </summary>
	/// <param name="WeaponID"></param>
	void Server_AddWeapon(TSubclassOf<AActor> WeaponID); //クライアント用

	/// <summary>
	/// インベントリの初期化
	/// </summary>
	void InventoryInitialize();

	/// <summary>
	/// インベントリに空きがあるか
	/// </summary>
	/// <returns></returns>
	bool HasEmptySlot() const;

    /// <summary>
    /// 引数の番号のインベントリから武器を取得
    /// </summary>
    /// <param name="index"></param>
    /// <returns></returns>
    const FInventoryWeapon* GetweaponSlot(int index) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// プレイヤーがマップにいるかの判定
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_InGamePlay)
	bool InGamePlay = false;

	UFUNCTION()
    void Server_DestructionWeaponPS(int index);

	// 2025.10.22 ウー start
	/// <summary>
	/// ポイント追加
	/// </summary>
	/// <param name="Value">量</param>
	UFUNCTION(Server, Reliable)
	void AddPoint(float Value);

	/// <summary>
	/// ポイント取得
	/// </summary>
	/// <returns></returns>
	float GetPoint() const;
	// 2025.10.22 ウー start

protected:

	UFUNCTION()
	void OnRep_Inventory();

	UFUNCTION()
	void OnRep_InGamePlay();
	

private:

	/// <summary>
	/// 名前
	/// </summary>
	FString Name;

	// 2025.10.22 ウー start
	/// <summary>
	/// ポイント同期
	/// </summary>
	UFUNCTION()
	void OnRep_UpdatePoint();

	/// <summary>
	/// ポイント
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_UpdatePoint)
	float Point;
	// 2025.10.22 ウー start
};

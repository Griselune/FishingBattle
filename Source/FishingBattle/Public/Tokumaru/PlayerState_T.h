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
UCLASS()
class FISHINGBATTLE_API APlayerState_T : public APlayerState
{
	GENERATED_BODY()

public:

	static const int slotMax = 4;

	UPROPERTY(Replicated,ReplicatedUsing = OnRep_Inventory)
	TArray<FInventoryWeapon> inventory;



	//プレイヤーステートクラスからレプリケート関数を動作させる意味がないので、
	// クライアント・サーバーで分ける必要がないが、一応念のため残しておく。
	// 一応レプリケートはできるらしいが、結局処理はプレイヤーからサーバー対応関数の中に
	// 入れてあるのでok
	//UFUNCTION(Server, Reliable, WithValidation)
	
	/// <summary>
	/// プレイヤーステート内のインベントリ配列に指定した文字列を入れる。現在四つまで（釣り竿含む）
	/// </summary>
	/// <param name="WeaponID"></param>
	void Server_AddWeapon(FName WeaponID); //クライアント用


	/// <summary>
	/// サーバー用に作ったが、そもそもオーナーを持たないプレイヤーステートでは意味がないので使っていない
	/// </summary>
	/// <param name="WeaponID"></param>
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_AddWeapon(FName WeaponID);  // サーバー用

	void InventoryInitialize();

	bool HasEmptySlot() const;

    const FInventoryWeapon* GetweaponSlot(int index) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// プレイヤーがマップにいるかの判定
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_InGamePlay)
	bool InGamePlay = false;

protected:

	UFUNCTION()
	void OnRep_Inventory();

	UFUNCTION()
	void OnRep_InGamePlay();
	
};

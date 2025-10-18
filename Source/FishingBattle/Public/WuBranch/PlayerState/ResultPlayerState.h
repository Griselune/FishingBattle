// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "WuBranch/PlayerState/ResultState.h"
#include "ResultPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API AResultPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	AResultPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	/// <summary>
	/// プレイヤーの結果を設定する
	/// </summary>
	/// <param name="NewState">結果</param>
	UFUNCTION(BlueprintCallable)
	void SetResultState(EResultState NewState);

	/// <summary>
	/// 結果がわかったですか
	/// </summary>
	/// <returns>true: はい, false: いいえ</returns>
	bool IsKnowResult() const;

	/// <summary>
	/// 勝者ですか
	/// </summary>
	/// <returns>true: はい, false: いいえ</returns>
	bool IsWinner() const;

	/// <summary>
	/// 敗者ですか
	/// </summary>
	/// <returns>true: はい, false: いいえ</returns>
	bool IsLoser() const;


private:

	/// <summary>
	/// 今の状態
	/// </summary>
	EResultState CurrentState;

#pragma region アニメショーン関連
public:
	/// <summary>
	/// 拍手してるですか
	/// </summary>
	/// <returns>true: はい, false: いいえ</returns>
	bool IsClapping() const;

	/// <summary>
	/// 拍手する
	/// </summary>
	void DoClap();

private:

	UFUNCTION()
	void OnRep_Clap();


	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Clap)
	bool bIsClapping;

	/*UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentState)
	bool bIsBigClapping;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentState)
	bool bIsCheering;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentState)
	bool bIsVictoryPose;*/

#pragma endregion
};

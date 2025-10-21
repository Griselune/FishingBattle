// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ResultPlayerAnimInstance.generated.h"

class AResultPlayer;

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API UResultPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UResultPlayerAnimInstance();

	/// <summary>
	/// アニメショーンの更新
	/// </summary>
	/// <param name="DeltaTime"></param>
	UFUNCTION(BlueprintCallable)
	void UpdateAnimation(float DeltaTime);

private:

	/// <summary>
	/// 対象を設定
	/// </summary>
	void SetOwningCharacter();

	/// <summary>
	/// 対象
	/// </summary>
	AResultPlayer* OwningCharacter;

	/// <summary>
	/// 開始するか
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	bool bIsStart;

	/// <summary>
	/// 勝者ですか
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	bool bIsWinner;
};

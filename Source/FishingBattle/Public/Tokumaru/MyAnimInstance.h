// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TakimotoBranch/CPPWeaponType.h"
#include "MyAnimInstance.generated.h"

class AFishingBattleCharacter;
/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	UMyAnimInstance();
public :
	UPROPERTY(BlueprintReadWrite)
	bool attack1;

	UPROPERTY(BlueprintReadWrite)
	bool Isjump;

	UPROPERTY(BlueprintReadWrite)
	bool IsFishing;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	ECPPWeaponType WeaponType;

	//UPROPERTY(BlueprintReadOnly,Category = "Character")
	//AFishingBattleCharacter* MyChar;

	UFUNCTION(BlueprintCallable)
	void GetWeaponType(ECPPWeaponType type_);

	UFUNCTION(BlueprintCallable)
	bool GetIsFishing();
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Tokumaru/WeaponType.h"
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

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::FishRod;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	AFishingBattleCharacter* MyChar;
	
};

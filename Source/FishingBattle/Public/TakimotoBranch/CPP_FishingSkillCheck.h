// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPP_FishingSkillCheck.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API UCPP_FishingSkillCheck : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	//バーの位置
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillCheck")
	float CurrentValue;

	//バーが動く最大位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillCheck")
	float MaxValue;

	//バーが動く最小位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillCheck")
	float MinValue;

	//バーが増えているかどうか
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillCheck")
	bool IsIncrease;

	//緑の枠の最大値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillCheck")
	float MaxGreenArea;

	//緑の枠の最小値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillCheck")
	float MinGreenArea;

public:

	//バーの動くスピード
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillCheck")
	float Speed = 0.f;

	//バーを止めるかどうか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillCheck")
	bool IsStop = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillCheck")
	bool IsClear = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillCheck")
	bool IsFishing = false;

	UFUNCTION(BlueprintCallable, Category = "SkillCheck")
	void BarMoving(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "SkillCheck")
	bool Stop();
};

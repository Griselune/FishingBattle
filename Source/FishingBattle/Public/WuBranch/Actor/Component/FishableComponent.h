// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FishableComponent.generated.h"

class UMeshComponent;
class ACPPBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FISHINGBATTLE_API UFishableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFishableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/// <summary>
	/// 魚をゲット
	/// </summary>
	/// <returns>魚</returns>
	ACPPBaseWeapon* GetFish();

private:

	/// <summary>
	/// 確率の合計を計算
	/// </summary>
	void CaculateTotalProbility();

	/// <summary>
	/// 釣れる魚の種類とその確率
	/// </summary>
	/// 魚のクラスができたら、ここを変更する
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<ACPPBaseWeapon>, int> FishTypes;

	/// <summary>
	/// 確率の合計
	/// </summary>
	int TotalProbility;

	/// <summary>
	/// 釣れた時のサウンド
	/// </summary>
	UPROPERTY(EditAnywhere)
	class USoundBase* CatchSound;
};

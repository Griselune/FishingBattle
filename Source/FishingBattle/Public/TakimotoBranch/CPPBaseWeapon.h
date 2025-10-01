// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TakimotoBranch/CPPWeaponType.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "CPPBaseWeapon.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class FISHINGBATTLE_API ACPPBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPPBaseWeapon();

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float NextHitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound;

	//2025/07/15 下に移動しました　得丸陽生
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	//ECPPWeaponType WeaponType;
	//------------------------------------

	virtual void Tick(float DeltaSeconds) override;

public:

	//2025/07/15 ここに移動しました　得丸陽生
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	ECPPWeaponType WeaponType;

	//武器を食べた時の回復量(攻撃力と反比例)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float HealingAmount;

	//武器を釣るのにかか1る時間(攻撃力が高い程時間がかかるようにする)
	//スキルチェックの要素を入れたためコメントアウト　9月30日14時55分　滝本海大
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FishingTime;*/

	//スキルチェックのバーの速さ(釣る難易度)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float SkillCheckSpeed;

	//アイコン画像
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UTexture2D* Icon;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void Attack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void OnHit(AActor* HitActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void Attack_Begin();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void Attack_Finish();

	virtual void BeginPlay() override;
};

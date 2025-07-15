// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TakimotoBranch/CPPWeaponType.h"
#include "Components/BoxComponent.h"
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

	//2025/07/15 下に移動しました　得丸陽生
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	//ECPPWeaponType WeaponType;
	//------------------------------------

	virtual void Tick(float DeltaSeconds) override;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void Attack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void OnHit(AActor* HitActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void Attack_Begin();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void Attack_Finish();

	//2025/07/15 ここに移動しました　得丸陽生
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Weapon")
	ECPPWeaponType WeaponType;

	virtual void BeginPlay() override;
};

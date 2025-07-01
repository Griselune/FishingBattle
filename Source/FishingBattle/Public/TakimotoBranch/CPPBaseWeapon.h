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
	ECPPWeaponType WeaponType;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void Attack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Weapon")
	void OnHit(AActor* HitActor);

};

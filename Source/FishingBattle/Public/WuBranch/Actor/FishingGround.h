// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FishingGround.generated.h"

class UBoxComponent;
class UFishableComponent;
class UWidgetComponent;

UCLASS()
class FISHINGBATTLE_API AFishingGround : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFishingGround();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// 魚を手に入れ
	/// </summary>
	/// <returns>魚</returns>
	ACPPBaseWeapon* GetFish();

private:
	
	/// <summary>
	/// 地面にいる釣れる範囲に入ったら
	/// </summary>
	UFUNCTION()
	void OnGroundAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// 地面にいる釣れる範囲を出たら
	/// </summary>
	UFUNCTION()
	void OnGroundAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// 地上の釣れる範囲
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* FishableAreaOnGround;

	/// <summary>
	/// インタラクションUI
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* InteractionUI;	

	/// <summary>
	/// 釣れる機能
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	UFishableComponent* Fishable;

	/// <summary>
	/// 魚がかかるまでの待ち時間
	/// </summary>
	UPROPERTY(EditAnywhere)
	float FishBiteDelay;
};

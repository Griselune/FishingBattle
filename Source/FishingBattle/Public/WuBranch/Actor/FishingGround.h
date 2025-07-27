// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FishingGround.generated.h"

class UBoxComponent;
class USphereComponent;
class UFishableComponent;
class UWidgetComponent;
class ACPPBaseWeapon;

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// 魚を手に入れ
	/// </summary>
	/// <returns>魚</returns>
	TSubclassOf<AActor> GetFish();

	/// <summary>
	/// 魚影ポイントをゲット
	/// </summary>
	/// <returns>魚影ポイント</returns>
	FVector GetFishingPointOnSea();

	/// <summary>
	/// 地上範囲の位置()を設定する
	/// </summary>
	/// <param name="Location">位置</param>
	UFUNCTION(Server, Reliable)
	void SetGroundLocation(FVector Location);

private:
	
	/// <summary>
	/// 地面にいる釣り範囲に入ったら
	/// </summary>
	UFUNCTION()
	void OnGroundAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// 地面にいる釣り範囲を出たら
	/// </summary>
	UFUNCTION()
	void OnGroundAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// 水上にいる釣り範囲に入ったら
	/// </summary>
	UFUNCTION()
	void OnSeaAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// 水上にいる釣り範囲を出たら
	/// </summary>
	UFUNCTION()
	void OnSeaAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// 地上範囲を同期する
	/// </summary>
	UFUNCTION()
	void OnRep_AsyncGround();

	/// <summary>
	/// 水上範囲を同期する
	/// </summary>
	UFUNCTION()
	void OnRep_AsyncSea();

	/// <summary>
	/// インタラクションUIを表示
	/// </summary>
	UFUNCTION(Client, Reliable)
	void ShowUI();

	/// <summary>
	/// インタラクションUIを非表示
	/// </summary>
	UFUNCTION(Client, Reliable)
	void CloseUI();

	/// <summary>
	/// 地上の釣り範囲
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	UBoxComponent* FishableAreaOnGround;
	
	/// <summary>
	/// 地上範囲の位置(同期用)
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_AsyncGround)
	FVector GroundLocation;

	/// <summary>
	/// 地上範囲のサイズ(同期用)
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_AsyncGround)
	FVector GroundSize;

	/// <summary>
	/// 水上の釣り範囲
	/// </summary>
	UPROPERTY(EditAnywhere, Replicated)
	USphereComponent* FishableAreaOnSea;
	
	/// <summary>
	/// 水上範囲の半径(同期用)
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_AsyncSea)
	float SeaRadius;

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

	/// <summary>
	/// 釣れた時のサウンド
	/// </summary>
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class USoundBase* CatchSound;
};

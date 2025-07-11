// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FishingGroundSpawnArea.generated.h"

class USphereComponent;

UCLASS()
class FISHINGBATTLE_API AFishingGroundSpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFishingGroundSpawnArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// 最遠距離の半径をゲット
	/// </summary>
	/// <returns>最遠距離の半径</returns>
	float GetFarthestRadius();

	/// <summary>
	/// 島の半径をゲット
	/// </summary>
	/// <returns>島の半径</returns>
	float GetIslandRadius();

private:

	/// <summary>
	/// 島の範囲
	/// </summary>
	UPROPERTY(EditAnywhere)
	USphereComponent* IslandArea;

	/// <summary>
	/// 最遠距離の範囲
	/// </summary>
	UPROPERTY(EditAnywhere)
	USphereComponent* FarthestArea;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SeaActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class FISHINGBATTLE_API ASeaActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASeaActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:

	/// <summary>
	/// 海に落ちたら
	/// </summary>
	UFUNCTION()
	void OnGroundAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// 海のメッシュ
	/// </summary>
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	/// <summary>
	/// 海の衝突範囲
	/// </summary>
	UPROPERTY(EditAnywhere)
	UBoxComponent* Collision;
};

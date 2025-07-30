// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CPPItemHeal.h"
#include "CPPHealItemSpawnLocation.generated.h"

UCLASS()
class FISHINGBATTLE_API ACPPHealItemSpawnLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPPHealItemSpawnLocation();

	float CurrentTime;
	float SpawnTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TObjectPtr<UBoxComponent> SpawnMark;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnClass")
	TSubclassOf<ACPPItemHeal> HealItemClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Spawn();
};

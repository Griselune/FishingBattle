// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TakimotoBranch/CPPBaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "CPPItemHeal.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API ACPPItemHeal : public ACPPBaseItem
{
	GENERATED_BODY()

private:

	bool IsHeal;

	float CurrentTime;

protected:

	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HealSound;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void Heal(AActor* Player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float SpawnTime;

public:

	void Tick(float DeltaTime) override;
};

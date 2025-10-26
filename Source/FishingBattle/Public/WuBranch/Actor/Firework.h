// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Firework.generated.h"

class UNiagaraComponent;
class UAudioComponent;

UCLASS()
class FISHINGBATTLE_API AFirework : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFirework();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// 花火を打ち上げる
	/// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void Fire();

private:

	/// <summary>
	/// 花火のエフェクト
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* FireworkEffect;

	/// <summary>
	/// 花火の音声コンポーネント
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* FireworkAudioComponent;
};

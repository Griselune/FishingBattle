// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Audio2DManager.generated.h"

class UAudioComponent;

UCLASS()
class FISHINGBATTLE_API AAudio2DManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudio2DManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:

	/// <summary>
	/// 歓声音声を再生
	/// </summary>
	UFUNCTION()
	void PlayCheerSound();

	/// <summary>
	/// 拍手音声を再生
	/// </summary>
	UFUNCTION()
	void PlayClapSound();

	/// <summary>
	/// 歓声音声コンポーネント
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* CheerAudioComponent;

	/// <summary>
	/// 拍手音声コンポーネント
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* ClapAudioComponent;
};

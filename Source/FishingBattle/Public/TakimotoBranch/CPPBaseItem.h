// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "CPPBaseItem.generated.h"

UCLASS()
class FISHINGBATTLE_API ACPPBaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPPBaseItem();

protected:

	void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;

public:

	void Tick(float DeltaTime) override;

};

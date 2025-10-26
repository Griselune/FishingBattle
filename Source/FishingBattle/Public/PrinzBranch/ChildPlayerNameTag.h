// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChildPlayerNameTag.generated.h"
class UWidgetComponent;

UCLASS()
class FISHINGBATTLE_API AChildPlayerNameTag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChildPlayerNameTag();

	// Getter for the widget component
	UWidgetComponent* GetNameTagWidgetComp() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Name Tag")
	UWidgetComponent* NameTagWidgetComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

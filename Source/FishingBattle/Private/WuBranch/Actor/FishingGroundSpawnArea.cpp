// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/FishingGroundSpawnArea.h"
#include "Components/SphereComponent.h"

// Sets default values
AFishingGroundSpawnArea::AFishingGroundSpawnArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	IslandArea = CreateDefaultSubobject<USphereComponent>(TEXT("Island area"));
	IslandArea->SetupAttachment(RootComponent);

	FarthestArea = CreateDefaultSubobject<USphereComponent>(TEXT("Farthest area"));
	FarthestArea->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFishingGroundSpawnArea::BeginPlay()
{
	Super::BeginPlay();
	
}

float AFishingGroundSpawnArea::GetFarthestRadius()
{
	return FarthestArea->GetScaledSphereRadius();
}

float AFishingGroundSpawnArea::GetIslandRadius()
{
	return IslandArea->GetScaledSphereRadius();
}

// Called every frame
//void AFishingGroundSpawnArea::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}


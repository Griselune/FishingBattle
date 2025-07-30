// Fill out your copyright notice in the Description page of Project Settings.


#include "TakimotoBranch/CPPHealItemSpawnLocation.h"

// Sets default values
ACPPHealItemSpawnLocation::ACPPHealItemSpawnLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root Component"));
	RootComponent = DefaultSceneRoot;

	SpawnMark = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Mark Component"));
	SpawnMark->SetupAttachment(RootComponent);

	SpawnMark->SetCollisionProfileName(FName("NoCollision"));
	SpawnMark->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ACPPHealItemSpawnLocation::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;

	Spawn();
	
	CurrentTime = 0.f;
	SpawnTime = 10.f;
}

// Called every frame
void ACPPHealItemSpawnLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;

	CurrentTime += DeltaTime;

	if (CurrentTime >= SpawnTime) {
		CurrentTime = 0.f;

		Spawn();
	}
}

void ACPPHealItemSpawnLocation::Spawn()
{
	if (HealItemClass) {
		FVector SpawnLocation = GetActorLocation();

		FRotator SpawnRotation(0.f, 0.f, 0.f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

		ACPPItemHeal* HealItem = GetWorld()->SpawnActor<ACPPItemHeal>(
			HealItemClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
		);

		UE_LOG(LogTemp, Warning, TEXT("Heal Item Spawn"));
	}
}
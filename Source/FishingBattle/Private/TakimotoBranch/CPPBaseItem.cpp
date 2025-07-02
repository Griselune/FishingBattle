// Fill out your copyright notice in the Description page of Project Settings.


#include "TakimotoBranch/CPPBaseItem.h"

// Sets default values
ACPPBaseItem::ACPPBaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultSceneRoot;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BoxCollision"));
	SphereCollision->SetupAttachment(RootComponent);

}
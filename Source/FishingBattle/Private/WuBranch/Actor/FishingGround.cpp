// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/FishingGround.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "WuBranch/Actor/Component/FishableComponent.h"


// Sets default values
AFishingGround::AFishingGround()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	FishableAreaOnGround = CreateDefaultSubobject<UBoxComponent>(TEXT("Fishable Area"));
	FishableAreaOnGround->SetupAttachment(RootComponent);

	InteractionUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction UI"));
	InteractionUI->SetupAttachment(RootComponent);

	Fishable = CreateDefaultSubobject<UFishableComponent>(TEXT("Fishable"));
}

// Called when the game starts or when spawned
void AFishingGround::BeginPlay()
{
	Super::BeginPlay();
	
	FishableAreaOnGround->OnComponentBeginOverlap.AddDynamic(this, &AFishingGround::OnGroundAreaBeginOverlap);
	FishableAreaOnGround->OnComponentEndOverlap.AddDynamic(this, &AFishingGround::OnGroundAreaEndOverlap);
}

// Called every frame
void AFishingGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AFishingGround::GetFish()
{
	return Fishable->GetFish();
}

void AFishingGround::OnGroundAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// プレイヤーの釣れるフラグを変更
	//　自分自身もプレイヤーに渡す

}

void AFishingGround::OnGroundAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// プレイヤーの釣れるフラグを変更

}

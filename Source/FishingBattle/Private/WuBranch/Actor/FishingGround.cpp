// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/FishingGround.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "WuBranch/Actor/Component/FishableComponent.h"
#include <FishingBattle/FishingBattleCharacter.h>
#include "TakimotoBranch/CPPBaseWeapon.h"

// Sets default values
AFishingGround::AFishingGround()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	FishableAreaOnGround = CreateDefaultSubobject<UBoxComponent>(TEXT("Fishable Area On Ground"));
	FishableAreaOnGround->SetupAttachment(RootComponent);

	FishableAreaOnSea = CreateDefaultSubobject<UBoxComponent>(TEXT("Fishable Area On Sea"));
	FishableAreaOnSea->SetupAttachment(RootComponent);

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
	FishableAreaOnSea->OnComponentBeginOverlap.AddDynamic(this, &AFishingGround::OnSeaAreaBeginOverlap);
	FishableAreaOnSea->OnComponentEndOverlap.AddDynamic(this, &AFishingGround::OnSeaAreaEndOverlap);
}

// Called every frame
void AFishingGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ACPPBaseWeapon* AFishingGround::GetFish()
{
	return Fishable->GetFish();
}

void AFishingGround::OnGroundAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// プレイヤーの釣れるフラグを変更
	//　自分自身もプレイヤーに渡す
	if (AFishingBattleCharacter* Player = Cast<AFishingBattleCharacter>(OtherActor))
	{
		Player->EnterSpot(this);
	}
}

void AFishingGround::OnGroundAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// プレイヤーの釣れるフラグを変更
	if (AFishingBattleCharacter* Player = Cast<AFishingBattleCharacter>(OtherActor))
	{
		Player->ExitSpot();
	}
}

void AFishingGround::OnSeaAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 釣り竿が入ったら
	if (false)
	{

	}
}

void AFishingGround::OnSeaAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 釣り竿が出たら
	if (false)
	{

	}
}

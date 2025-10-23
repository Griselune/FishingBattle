// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/FishingGround.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "WuBranch/Actor/Component/FishableComponent.h"
#include <Net/UnrealNetwork.h>
#include <FishingBattle/FishingBattleCharacter.h>
#include "TakimotoBranch/CPPBaseWeapon.h"
#include <WuBranch/UI/MyBaseWidget.h>
#include <Kismet/GameplayStatics.h>
#include "NiagaraComponent.h"

// Sets default values
AFishingGround::AFishingGround()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	FishableAreaOnGround = CreateDefaultSubobject<UBoxComponent>(TEXT("Fishable Area On Ground"));
	FishableAreaOnGround->SetIsReplicated(true);
	FishableAreaOnGround->SetupAttachment(RootComponent);

	FishableAreaOnSea = CreateDefaultSubobject<USphereComponent>(TEXT("Fishable Area On Sea"));
	FishableAreaOnSea->SetIsReplicated(true);
	FishableAreaOnSea->SetupAttachment(RootComponent);

	InteractionUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction UI"));
	InteractionUI->SetupAttachment(RootComponent);

	ExistenceEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Existence Effect"));
	ExistenceEffect->SetupAttachment(RootComponent);

	ExistenceInvisibilityArea = CreateDefaultSubobject<USphereComponent>(TEXT("Existence Invisibility Area"));
	ExistenceInvisibilityArea->SetupAttachment(RootComponent);

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
	ExistenceInvisibilityArea->OnComponentBeginOverlap.AddDynamic(this, &AFishingGround::OnExistenceInvisibilityAreaBeginOverlap);
	ExistenceInvisibilityArea->OnComponentEndOverlap.AddDynamic(this, &AFishingGround::OnExistenceInvisibilityAreaEndOverlap);

	CloseUI();
	ShowExistenceEffect();
}

// Called every frame
//void AFishingGround::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AFishingGround::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// レプリケーションするプロパティを追加
	DOREPLIFETIME(AFishingGround, FishableAreaOnGround);
	DOREPLIFETIME(AFishingGround, GroundLocation);
	DOREPLIFETIME(AFishingGround, GroundSize);
	DOREPLIFETIME(AFishingGround, FishableAreaOnSea);
	DOREPLIFETIME(AFishingGround, SeaRadius);
}

TSubclassOf<AActor> AFishingGround::GetFish()
{
	TSubclassOf<AActor> Fish = Fishable->GetFish();

	// サウンド
	if (CatchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CatchSound, GetFishingPointOnSea());
	}
	return Fish;
}

FVector AFishingGround::GetFishingPointOnSea()
{
	return FishableAreaOnSea->GetComponentLocation();
}

void AFishingGround::SetGroundLocation_Implementation(FVector Location)
{
	GroundLocation = Location;
}

void AFishingGround::OnGroundAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// プレイヤーの釣れるフラグを変更
	//　自分自身もプレイヤーに渡す
	if (AFishingBattleCharacter* Player = Cast<AFishingBattleCharacter>(OtherActor))
	{
		if (Player && Player->IsLocallyControlled())
		{
			Player->EnterSpot(this);
			ShowUI();
		}
	}
}

void AFishingGround::OnGroundAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// プレイヤーの釣れるフラグを変更
	if (AFishingBattleCharacter* Player = Cast<AFishingBattleCharacter>(OtherActor))
	{
		if (Player && Player->IsLocallyControlled())
		{
			Player->ExitSpot();
			CloseUI();
		}
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

void AFishingGround::OnExistenceInvisibilityAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AFishingBattleCharacter* Player = Cast<AFishingBattleCharacter>(OtherActor))
	{
		if (Player && Player->IsLocallyControlled())
		{
			CloseExistenceEffect();
		}
	}
}

void AFishingGround::OnExistenceInvisibilityAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AFishingBattleCharacter* Player = Cast<AFishingBattleCharacter>(OtherActor))
	{
		if (Player && Player->IsLocallyControlled())
		{
			ShowExistenceEffect();
		}
	}
}

void AFishingGround::OnRep_AsyncGround()
{
	FishableAreaOnGround->SetRelativeLocation(GroundLocation);
	FishableAreaOnGround->SetBoxExtent(GroundSize);
}

void AFishingGround::OnRep_AsyncSea()
{
	FishableAreaOnSea->SetSphereRadius(SeaRadius);
}

void AFishingGround::ShowExistenceEffect_Implementation()
{
	ExistenceEffect->Activate(true);
	ExistenceEffect->SetVisibility(true);
}

void AFishingGround::CloseExistenceEffect_Implementation()
{
	ExistenceEffect->Deactivate();
	ExistenceEffect->SetVisibility(false);
}

void AFishingGround::ShowUI_Implementation()
{
	if (UMyBaseWidget* UIWidget = Cast<UMyBaseWidget>(InteractionUI->GetWidget()))
	{
		UIWidget->ShowUI();
	}
}

void AFishingGround::CloseUI_Implementation()
{
	if (UMyBaseWidget* UIWidget = Cast<UMyBaseWidget>(InteractionUI->GetWidget()))
	{
		UIWidget->CloseUI();
	}
}

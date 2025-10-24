// Fill out your copyright notice in the Description page of Project Settings.


#include "TakimotoBranch/CPPBaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ACPPBaseWeapon::ACPPBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultSceneRoot;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
}

void ACPPBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	NextHitTime = 0.f;
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	BoxCollision->SetCollisionProfileName(TEXT("NoCollision"));

	//2025 7/15 新しくスポーンさせたときにエディタの変更が上書きされるので、いったん
	// コメントアウトしました。 得丸陽生
	//WeaponType = ECPPWeaponType::None;
}

void ACPPBaseWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	NextHitTime--;
	if (NextHitTime <= 0) {
		NextHitTime = 0;
	}
}

void ACPPBaseWeapon::Attack_Implementation()
{

}

void ACPPBaseWeapon::OnHit_Implementation(AActor* HitActor)
{
	//if (!HasAuthority()) return; //クライアントだったらreturnする
	//if (!HitActor->ActorHasTag(FName("Player"))) return; //当たったのがプレイヤー以外だったらreturn

	if (HitActor && HitActor != GetOwner()) {
		if (NextHitTime <= 0){
			// 2025.10.23 ウー start
			//UGameplayStatics::ApplyDamage(HitActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
			UGameplayStatics::ApplyDamage(HitActor, Damage, GetInstigatorController(), GetOwner(), UDamageType::StaticClass());
			// 2025.10.23 ウー end
			if(HitSound) UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
			UE_LOG(LogTemp, Error, TEXT("Hit!"));
			NextHitTime = 30.f;
		}
	}
}

void ACPPBaseWeapon::Attack_Begin_Implementation()
{
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAll"));
}

void ACPPBaseWeapon::Attack_Finish_Implementation()
{
	BoxCollision->SetCollisionProfileName(TEXT("NoCollision"));
}

// 2025.10.22 ウー start
float ACPPBaseWeapon::GetPoint() const
{
	return Point;
}
// 2025.10.22 ウー end
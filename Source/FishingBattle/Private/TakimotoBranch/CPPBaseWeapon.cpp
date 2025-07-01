// Fill out your copyright notice in the Description page of Project Settings.


#include "TakimotoBranch/CPPBaseWeapon.h"
#include "Kismet/GameplayStatics.h"

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

	Damage = 0.f;

	WeaponType = ECPPWeaponType::Non;
}

void ACPPBaseWeapon::Attack_Implementation()
{

}

void ACPPBaseWeapon::OnHit_Implementation(AActor* HitActor)
{
	if (HitActor && HitActor != this) {
		UGameplayStatics::ApplyDamage(HitActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		UE_LOG(LogTemp, Error, TEXT("Hit!"));
	}
}
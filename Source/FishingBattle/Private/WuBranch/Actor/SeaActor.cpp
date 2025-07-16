// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/SeaActor.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <FishingBattle/FishingBattleCharacter.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Engine/DamageEvents.h>

// Sets default values
ASeaActor::ASeaActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// 海の衝突範囲を作成
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("SeaCollision"));
	RootComponent = Collision;

	// 海のメッシュを作成
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SeaMesh"));
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASeaActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 衝突イベントのバインド
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ASeaActor::OnGroundAreaBeginOverlap);
}

// Called every frame
//void ASeaActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ASeaActor::OnGroundAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (AFishingBattleCharacter* Player = Cast<AFishingBattleCharacter>(OtherActor))
		{
			// プレイヤーが海に落ちた場合の処理
			if (Player->GetCharacterMovement())
			{
				// プレイヤーの移動を停止
				Player->GetCharacterMovement()->StopMovementImmediately();
				
				// 即死
				Player->TakeDamage(100.0f, FDamageEvent(), nullptr, this);
			}
		}
	}
}
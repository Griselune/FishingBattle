// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/Firework.h"
#include "NiagaraComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/AudioComponent.h"

// Sets default values
AFirework::AFirework()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	FireworkEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Firework Effect"));
	FireworkEffect->SetupAttachment(RootComponent);
	FireworkEffect->SetAutoActivate(false);

	FireworkAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Firework Audio Component"));
	FireworkAudioComponent->SetupAttachment(RootComponent);
	FireworkAudioComponent->bAutoActivate = false;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AFirework::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void AFirework::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

void AFirework::Fire_Implementation()
{
	// クライアント側でエフェクトを再生
	if(FireworkEffect)
	{
		FireworkEffect->Activate(true);
	}
	// クライアント側で音声を再生
	if(FireworkAudioComponent && !FireworkAudioComponent->IsPlaying())
	{
		FireworkAudioComponent->Play();
	}
}


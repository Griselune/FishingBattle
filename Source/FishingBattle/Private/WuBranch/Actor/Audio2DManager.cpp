// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/Audio2DManager.h"
#include "Components/AudioComponent.h"
#include <Kismet/GameplayStatics.h>
#include "WuBranch/Actor/ResultSpotLight.h"

// Sets default values
AAudio2DManager::AAudio2DManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    CheerAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("CheerAudioComponent"));
    CheerAudioComponent->SetupAttachment(RootComponent);
    CheerAudioComponent->bAutoActivate = false;

    ClapAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ClapAudioComponent"));
    ClapAudioComponent->SetupAttachment(RootComponent);
    ClapAudioComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AAudio2DManager::BeginPlay()
{
	Super::BeginPlay();
	
    TArray<AActor*> SpotLights;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AResultSpotLight::StaticClass(), SpotLights);
    if (SpotLights.Num() > 0)
    {
        AResultSpotLight* SpotLightActor = Cast<AResultSpotLight>(SpotLights[0]);
        if (SpotLightActor)
        {
			UE_LOG(LogTemp, Warning, TEXT("Audio2DManager: Found ResultSpotLight actor and binding event."));
            SpotLightActor->OnPointed.AddDynamic(this, &AAudio2DManager::PlayCheerSound);
        }
	}

    CheerAudioComponent->OnAudioFinished.AddDynamic(this, &AAudio2DManager::PlayClapSound);
}

// Called every frame
//void AAudio2DManager::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AAudio2DManager::PlayCheerSound()
{
    if (CheerAudioComponent->Sound && !CheerAudioComponent->IsPlaying())
    {
        CheerAudioComponent->Play();
    }
}

void AAudio2DManager::PlayClapSound()
{
    if (ClapAudioComponent->Sound && !ClapAudioComponent->IsPlaying())
    {
        ClapAudioComponent->Play();
    }
}
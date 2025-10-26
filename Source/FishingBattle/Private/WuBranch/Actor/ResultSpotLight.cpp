// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/ResultSpotLight.h"
#include "Components/SpotLightComponent.h"
#include "Components/AudioComponent.h"
#include <Kismet/GameplayStatics.h>
#include "WuBranch/Actor/ResultPlayer.h"
#include <Kismet/KismetMathLibrary.h>
#include <Net/UnrealNetwork.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
AResultSpotLight::AResultSpotLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	SpotLight->SetupAttachment(RootComponent);
    SpotLight->SetIsReplicated(true);

	PreRevealAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PreRevealAudioComponent"));
	PreRevealAudioComponent->SetupAttachment(RootComponent);
	PreRevealAudioComponent->bAutoActivate = false;

    bReplicates = true;
}

// Called when the game starts or when spawned
void AResultSpotLight::BeginPlay()
{
	Super::BeginPlay();

    CanStart = false;
    bIsEffectFinish = false;
    MovingTimeCnt = 0;
}

// Called every frame
void AResultSpotLight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // サーバーだけ
    if (HasAuthority())
    {
        CountEffectTime(DeltaTime);
        MoveToNewTarget(DeltaTime);
    }
}

void AResultSpotLight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AResultSpotLight, SpotLight);
	//DOREPLIFETIME(AResultSpotLight, WinnerID);
	DOREPLIFETIME(AResultSpotLight, CanStart);
}

void AResultSpotLight::StartMove_Implementation()
{
    // サーバーだけ
    if (!HasAuthority())
        return;

    // すべてのプレイヤーキャラクターをゲット
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AResultPlayer::StaticClass(), Characters);

    SetFirstLightTarget();

    // 隨機目標
    NextTarget = GetNewRandomTarget();

    // 開始移動
    CanStart = true;

	// プレリビール音声を再生
    PlayPreRevealSound();
}

void AResultSpotLight::SetWinner_Implementation(int ID)
{
	WinnerID = ID;
}

void AResultSpotLight::OnRep_UpdateCanStart()
{
    if (CanStart)
    {
        // プレリビール音声を再生
        PlayPreRevealSound();
	}
    else
    {
        NotifyPointedWinnerEvent();
    }
}

void AResultSpotLight::SetFirstLightTarget()
{
    CurrentTarget = GetNewRandomTarget();
    if(!CurrentTarget)
		return;
	CurrentTargetLocation = CurrentTarget->GetActorLocation();
    PointLightToLocation(CurrentTargetLocation);
}

AActor* AResultSpotLight::GetNewRandomTarget()
{
    if (Characters.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Error! No Player Data"));
        return nullptr;
    }

    // 一つだけの場合、直接0の方をあげる
    if (Characters.Num() == 1)
    {
        return Characters[0];
    }

    // キャラクターリストのコピーを作る
    AActor* NewTarget;
	TArray<AActor*> CharacterCopy = Characters;
	// 現在の目標をリストから外す
	CharacterCopy.Remove(CurrentTarget);
	// 隨機に選ぶ
    int Index = FMath::RandRange(0, CharacterCopy.Num() - 1);
    NewTarget = Characters[Index];
    return NewTarget;
}

void AResultSpotLight::CountEffectTime(float DeltaTime)
{
	// 開始していない場合、または停止が必要な場合、直接戻る
    if (!CanStart)
        return;

    if (bIsEffectFinish)
        return;

	// 演出時間が過ぎたので、強制的に最後の目標を設定
    if (EffectTimeCnt >= EffectDuration)
    {
        bIsEffectFinish = true;
        ForceSetLastTarget(FindWinnerCharacter());
		// プレリビール音声を停止
		StopPreRevealSound();
        return;
    }

    EffectTimeCnt += DeltaTime;
}

void AResultSpotLight::MoveToNewTarget(float DeltaTime)
{
    if (!CanStart)
        return;

    if(!NextTarget)
		return;

    //FVector Start = CurrentTarget->GetActorLocation();
    FVector Start = CurrentTargetLocation;
    FVector End = NextTarget->GetActorLocation();

	MovingTimeCnt += DeltaTime;
	float Alpha = MovingTimeCnt / MovingDuration;    

    if (Alpha <= 1.0f)
    {
		// 移動中
        //FVector Location = FMath::Lerp(Start, End, Alpha);
		//PointLightToLocation(Location);

        CurrentTargetLocation = FMath::Lerp(Start, End, Alpha);
        PointLightToLocation(CurrentTargetLocation);
    }
    else
    {
        // 移動完了
        CurrentTarget = NextTarget;
        MovingTimeCnt = 0.0f;
        // 演出時間が終了だった場合
        if (bIsEffectFinish)
        {
            NotifyPointedWinnerEvent();
            CanStart = false;
        }
        else
        {
            // 次の目標を隨機にゲット
            NextTarget = GetNewRandomTarget();
        }   
    }
}

void AResultSpotLight::PointLightToLocation(FVector TargetLocation)
{
	FVector StartLocation = SpotLight->GetComponentLocation();
    FRotator Angle = (TargetLocation - StartLocation).Rotation();
    //FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(SpotLight->GetComponentLocation(), TargetLocation);
    SpotLight->SetWorldRotation(Angle);
}

void AResultSpotLight::ForceSetLastTarget(AActor* Target)
{
    if (!Target)
        return;

    NextTarget = Target;
    MovingTimeCnt = 0.0f;
}

AActor* AResultSpotLight::FindWinnerCharacter()
{
    for (AActor* Actor : Characters)
    {
        AResultPlayer* Player = Cast<AResultPlayer>(Actor);
        if (Player && Player->GetUUID() == WinnerID)
        {
            return Player;
        }
	}
	return nullptr;
}

void AResultSpotLight::NotifyPointedWinnerEvent()
{
	if (OnPointed.IsBound())
        OnPointed.Broadcast();
}

void AResultSpotLight::PlayPreRevealSound_Implementation()
{
    if(PreRevealAudioComponent->Sound && !PreRevealAudioComponent->IsPlaying())
    {
        PreRevealAudioComponent->Play();
	}
}

void AResultSpotLight::StopPreRevealSound_Implementation()
{
    if (PreRevealAudioComponent->Sound && PreRevealAudioComponent->IsPlaying())
    {
		PreRevealAudioComponent->Stop();
    }
}


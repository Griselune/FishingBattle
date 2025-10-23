// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/GameMode/ResultMode.h"
#include "WuBranch/Library/MyGameplaySystemLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/PlayerStart.h"
#include "PrinzBranch/LANGameInstance.h"
#include <Kismet/KismetMathLibrary.h>
#include "WuBranch/Actor/ResultPlayer.h"
#include "WuBranch/Actor/ResultSpotLight.h"
#include "WuBranch/Actor/Firework.h"
#include <WuBranch/PlayerController/ResultPlayerController.h>

AResultMode::AResultMode()
	: CameraLocation(FVector::ZeroVector)
	, SpotLightNumber(0)
{

}

void AResultMode::StartPlay()
{	
	// 先に勝者を見つける、ここでやらないとPlayerControllerで勝者の名前をリクエストしたときにまだ見つかってない可能性がある
	// ゲームインスタンスをゲット
	MyGameInstance = GetGameInstance<ULANGameInstance>();

	// 偽資料
	/*MyGameInstance->AddRecord(FPlayerRecord{ 1, 0, 0, "aaa" });
	MyGameInstance->AddRecord(FPlayerRecord{ 2, 1, 0, "bbb" });
	MyGameInstance->AddRecord(FPlayerRecord{ 3, 0, 5, "ccc" });*/

	// 勝者を探す
	TArray<FPlayerRecord> Records = MyGameInstance->GetRecords();
	FPlayerRecord* Winner = UMyGameplaySystemLibrary::FindWinner(Records);
	if(!Winner)
	{
		UE_LOG(LogTemp, Error, TEXT("Did not find Winner"));
	}
	else
	{
		WinnerID = Winner->ID;
		WinnerName = Winner->Name;
	}

	Super::StartPlay();

	// カメラの位置をゲット
	AActor* Start = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
	CameraLocation = Start->GetActorLocation();

	// 全プレイヤーのキャラクターを生成
	SpawnPlayer();

	// 照明灯の設定
	InitSpotLight();

	// 花火を探す
	FindFireworks();
}

FString AResultMode::GetWinnerName() const
{
	return WinnerName;
}

void AResultMode::SpawnPlayer()
{
	if (!MyGameInstance)
		return;

	TArray<FPlayerRecord> Records = MyGameInstance->GetRecords();
	// プレイヤーの幅さをゲット
	if (!PlayerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Did not set Player!"));
		return;
	}
	AResultPlayer* PlayerTemp = PlayerClass->GetDefaultObject<AResultPlayer>();
	float PlayerWidth = PlayerTemp->GetCollisionWidth();

	for (const FPlayerRecord& Record : Records)
	{
		// 生成する位置と角度をゲット
		FVector Location = GetSpawnLocation(PlayerWidth);
		FRotator Angle = UKismetMathLibrary::FindLookAtRotation(Location, CameraLocation);
		Angle.Roll = 0.f;
		Angle.Pitch = 0.f;
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		// キャラクターを生成
		AResultPlayer* Character = GetWorld()->SpawnActor<AResultPlayer>(PlayerClass, Location, Angle, SpawnParams);

		if (Character)
		{
			Character->SetUUID(Record.ID);
			Character->SetName(Record.Name);
			PlayerCharacters.Add(Character);
		}
	}
}

FVector AResultMode::GetSpawnLocation(float PlayerWidth)
{
	FVector Location;
	bool bIsHit;
	FHitResult HitResult;
	do
	{
		Location = UKismetMathLibrary::RandomPointInBoundingBox(PlayerSpawnCenter, PlayerSpawnHalfSize);

		FVector Start = Location + FVector(0, 0, 1000);
		FVector End = Location + FVector(0, 0, -100);
		FCollisionShape Shape = FCollisionShape::MakeSphere(PlayerWidth);
		
		bIsHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Pawn, Shape);
	} while (!HitResult.GetActor()->ActorHasTag("Floor"));
	
	return Location;
}

void AResultMode::InitSpotLight()
{
	// すべての照明灯をゲット
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AResultSpotLight::StaticClass(), SpotLights);
	if (!SpotLights.IsEmpty())
	{
		for (AActor* Actor : SpotLights)
		{
			AResultSpotLight* SpotLight = Cast<AResultSpotLight>(Actor);
			if (SpotLight)
			{
				SpotLight->OnPointed.AddDynamic(this, &AResultMode::OnPointedWinner);
				SpotLight->SetWinner(WinnerID);
				SpotLight->StartMove();
			}
		}
	}
	SpotLightNumber = 0;
}

void AResultMode::OnPointedWinner()
{
	SpotLightNumber++;
	// 照明灯全部が終わったら
	if (SpotLightNumber >= SpotLights.Num())
	{
		// 花火を打ち上げる
		for (AFirework* Firework : Fireworks)
		{
			Firework->Fire();
		}

		// 勝者の名前を表示
		ShowWinnerName();

		// プレイヤーのアニメーション状態を開始
		StartPlayerAnimationState();
	}
}

void AResultMode::FindFireworks()
{
	TArray<AActor*> FireworkActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFirework::StaticClass(), FireworkActors);
	for(AActor* Actor : FireworkActors)
	{
		AFirework* Firework = Cast<AFirework>(Actor);
		if(Firework)
		{
			Fireworks.Add(Firework);
		}
	}
}

void AResultMode::ShowWinnerName()
{
	// すべてのプレイヤーのHUDを表示させる
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC)
		{
			if (AResultPlayerController* ResultPC = Cast<AResultPlayerController>(PC))
			{
				ResultPC->ShowWinnerName();
			}
		}
	}
}

void AResultMode::StartPlayerAnimationState()
{
	for (AResultPlayer* Character : PlayerCharacters)
	{
		if (Character->GetUUID() == WinnerID)
			Character->SetResultState(EResultState::Winner);
		else
			Character->SetResultState(EResultState::Loser);
	}
}

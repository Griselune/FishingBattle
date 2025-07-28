// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/Component/FishableComponent.h"
#include "Components/MeshComponent.h"
#include "TakimotoBranch/CPPBaseWeapon.h"

// Sets default values for this component's properties
UFishableComponent::UFishableComponent()
	: TotalProbility(0.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFishableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	CaculateTotalProbility();
}


// Called every frame
//void UFishableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

TSubclassOf<AActor> UFishableComponent::GetFish()
{
	// １から合計確率までランダムで数値をゲット
	int Target = FMath::RandRange(1, TotalProbility);
	// 
	auto FishTypesArray = FishTypes.Array();
	int Sum = 0;
	TSubclassOf<AActor> Fish = nullptr;
	for (const auto& FishType : FishTypesArray)
	{
		Sum += FishType.Value;
		if (Sum >= Target)
		{
			//ACPPBaseWeapon* Weapon = NewObject<ACPPBaseWeapon>(GetWorld(), FishType.Key.Get());
			Fish = FishType.Key;
		}
			
	}
		
	return Fish;
}

void UFishableComponent::CaculateTotalProbility()
{
	// 合計確率を初期化
	TotalProbility = 0.0f;

	// Mapに内容がない場合
	if (FishTypes.Num() == 0)
		return;

	// 合計を計算
	TArray<int> Probilities;
	FishTypes.GenerateValueArray(Probilities);
	for (const int& Probility : Probilities)
	{
		TotalProbility += Probility;
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "TakimotoBranch/CPPItemHeal.h"
#include "FishingBattle/FishingBattleCharacter.h"

void ACPPItemHeal::BeginPlay()
{
	Super::BeginPlay();

	CurrentTime = 0.f;
}

void ACPPItemHeal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsHeal) {
		CurrentTime += DeltaTime;

		if (CurrentTime >= SpawnTime) {
			CurrentTime = 0.f;

			IsHeal = false;

			StaticMesh->SetVisibility(true);
		}
	}
}

void ACPPItemHeal::Heal(AActor* Player)
{
	AFishingBattleCharacter* fbc = Cast<AFishingBattleCharacter>(Player);
	if (fbc) {
		if (IsHeal == false) {
			IsHeal = true;

			if (!HasAuthority()) return;
			// 2025.07.17 ウー start
			//fbc->Health += 10.f;
			fbc->Heal(10.0f);
			// 2025.07.17 ウー end

			if (HealSound) UGameplayStatics::PlaySoundAtLocation(this, HealSound, GetActorLocation());

			StaticMesh->SetVisibility(false);

			UE_LOG(LogTemp, Error, TEXT("Heal"));
		}
	}
}
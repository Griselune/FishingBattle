// Fill out your copyright notice in the Description page of Project Settings.


#include "TakimotoBranch/CPPItemHeal.h"
#include "FishingBattle/FishingBattleCharacter.h"

void ACPPItemHeal::Heal(AActor* Player)
{
	if (IsHeal == false) {
		IsHeal = true;
		AFishingBattleCharacter* fbc = Cast<AFishingBattleCharacter>(Player);
		if (fbc)
		{
			if (!HasAuthority()) return;
			// 2025.07.17 ウー start
			//fbc->Health += 10.f;
			fbc->Heal(10.0f);
			// 2025.07.17 ウー end

			if(HealSound) UGameplayStatics::PlaySoundAtLocation(this, HealSound, GetActorLocation());

			UE_LOG(LogTemp, Error, TEXT("Heal"));

			this->Destroy();
		}
	}
}
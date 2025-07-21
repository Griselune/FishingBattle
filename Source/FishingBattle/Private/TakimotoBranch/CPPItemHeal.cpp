// Fill out your copyright notice in the Description page of Project Settings.


#include "TakimotoBranch/CPPItemHeal.h"
#include "FishingBattle/FishingBattleCharacter.h"

void ACPPItemHeal::Heal(AActor* Player)
{
	AFishingBattleCharacter* fbc = Cast<AFishingBattleCharacter>(Player);
	if (fbc)
	{
		// 2025.07.17 ウー start
		//fbc->Health += 10.f;
		fbc->Heal(10.0f);
		// 2025.07.17 ウー end
		UE_LOG(LogTemp, Error, TEXT("Heal"));

		this->Destroy();
	}
}
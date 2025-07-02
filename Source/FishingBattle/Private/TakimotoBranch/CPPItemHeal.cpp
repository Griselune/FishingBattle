// Fill out your copyright notice in the Description page of Project Settings.


#include "TakimotoBranch/CPPItemHeal.h"
#include "FishingBattle/FishingBattleCharacter.h"

void ACPPItemHeal::Heal(AActor* Player)
{
	AFishingBattleCharacter* fbc = Cast<AFishingBattleCharacter>(Player);
	if (fbc)
	{
		fbc->Health += 10.f;
		UE_LOG(LogTemp, Error, TEXT("Heal"));

		this->Destroy();
	}
}
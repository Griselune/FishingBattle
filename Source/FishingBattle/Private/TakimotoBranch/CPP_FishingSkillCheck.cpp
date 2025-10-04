// Fill out your copyright notice in the Description page of Project Settings.


#include "TakimotoBranch/CPP_FishingSkillCheck.h"

void UCPP_FishingSkillCheck::BarMoving(float DeltaTime)
{
	if (IsStop == true) return;
	if (IsIncrease == true) {
		if (CurrentValue >= MaxValue) IsIncrease = false;
		CurrentValue += Speed * DeltaTime;
	}
	else {
		if (CurrentValue <= MinValue) IsIncrease = true;
		CurrentValue -= Speed * DeltaTime;
	}
}

bool UCPP_FishingSkillCheck::Stop()
{
	if (IsStop)IsStop = false;
	else IsStop = true;

	if (MinGreenArea <= CurrentValue && CurrentValue <= MaxGreenArea) {
		UE_LOG(LogTemp, Warning, TEXT("Clear"));
		return true;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed"));
		return false;
	}
}
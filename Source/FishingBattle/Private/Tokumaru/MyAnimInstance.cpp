// Fill out your copyright notice in the Description page of Project Settings.

#include "FishingBattle/FishingBattleCharacter.h"
#include "Tokumaru/MyAnimInstance.h"

UMyAnimInstance::UMyAnimInstance() {
	attack1 = false;
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Owner = TryGetPawnOwner();
	if (!Owner) return;

	MyChar = Cast<AFishingBattleCharacter>(Owner);
	if (!MyChar) return;

	//WeaponType = MyChar->CurrentWeaponType;
}


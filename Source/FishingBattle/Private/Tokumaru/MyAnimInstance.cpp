// Fill out your copyright notice in the Description page of Project Settings.
#include "Tokumaru/MyAnimInstance.h"
#include "FishingBattle/FishingBattleCharacter.h"


UMyAnimInstance::UMyAnimInstance() {
	attack1 = false;
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UMyAnimInstance::GetWeaponType(ECPPWeaponType type_)
{
	//APawn* Owner = TryGetPawnOwner();
	//if (Owner) {
	//	MyChar = Cast<AFishingBattleCharacter>(Owner);
	//	if (MyChar) {
	//		this->WeaponType = MyChar->WeaponType;
	//		OnRep_AnimInstance();
	//	}
	//}
	WeaponType = type_;
	return;
}



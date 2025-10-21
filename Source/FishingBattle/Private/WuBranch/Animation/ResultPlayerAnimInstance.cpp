// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Animation/ResultPlayerAnimInstance.h"
#include "WuBranch/Actor/ResultPlayer.h"
#include <WuBranch/PlayerState/ResultPlayerState.h>

UResultPlayerAnimInstance::UResultPlayerAnimInstance()
	: Super()
	, OwningCharacter(nullptr)
	, bIsStart(false)
	, bIsWinner(false)
{
}

void UResultPlayerAnimInstance::UpdateAnimation(float DeltaTime)
{
	if (!OwningCharacter)
		SetOwningCharacter();

	if (OwningCharacter)
	{
		bIsStart = OwningCharacter->IsKnowResult();
		bIsWinner = OwningCharacter->IsWinner();
	}
}

void UResultPlayerAnimInstance::SetOwningCharacter()
{
	if(!OwningCharacter)
	{
		OwningCharacter = Cast<AResultPlayer>(TryGetPawnOwner());
	}
}

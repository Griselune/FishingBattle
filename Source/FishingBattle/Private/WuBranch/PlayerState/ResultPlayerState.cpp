// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/PlayerState/ResultPlayerState.h"

AResultPlayerState::AResultPlayerState()
{
}

void AResultPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


}

void AResultPlayerState::SetResultState(EResultState NewState)
{
	CurrentState = NewState;
}

bool AResultPlayerState::IsKnowResult() const
{
	return CurrentState != EResultState::Wait;
}

bool AResultPlayerState::IsWinner() const
{
	return CurrentState == EResultState::Winner;
}

bool AResultPlayerState::IsLoser() const
{
	return CurrentState == EResultState::Loser;
}

bool AResultPlayerState::IsClapping() const
{
	return bIsClapping;
}

void AResultPlayerState::DoClap()
{
	bIsClapping = true;
}

void AResultPlayerState::OnRep_Clap()
{

}

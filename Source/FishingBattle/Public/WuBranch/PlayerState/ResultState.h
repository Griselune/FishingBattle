// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResultState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EResultState : uint8
{
	Wait,
	Loser,
	Winner
};

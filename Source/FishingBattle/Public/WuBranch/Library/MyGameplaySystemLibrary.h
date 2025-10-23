// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyGameplaySystemLibrary.generated.h"

struct FPlayerRecord;

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API UMyGameplaySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// 優勝者を見つける
	/// </summary>
	/// <param name="Records">記録</param>
	static FPlayerRecord* FindWinner(TArray<FPlayerRecord>& Records);

private:

	/// <summary>
	/// 優勝候補者を見つける
	/// </summary>
	/// <param name="Records">記録</param>
	/// <returns></returns>
	static TArray<FPlayerRecord> FindPotentials(TArray<FPlayerRecord>& Records);
};

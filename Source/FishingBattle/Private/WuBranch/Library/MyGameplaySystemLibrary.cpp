// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Library/MyGameplaySystemLibrary.h"
#include "WuBranch/Struct/PlayerRecord.h"

FPlayerRecord* UMyGameplaySystemLibrary::FindWinner(TArray<FPlayerRecord>& Records)
{
	TArray<FPlayerRecord> Potentials = FindPotentials(Records);
	// 誰もいない
	if (Potentials.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Did not find Potentials"));
		return nullptr;
	}
	// 二人以上の場合
	if (Potentials.Num() >= 2)
	{
		// ダメージで並べ替え(降順)
		Potentials.Sort([](const FPlayerRecord& Record1, const FPlayerRecord& Record2) {
			return Record1.TotalDamage > Record2.TotalDamage;
			});
	}

	// １人目が勝者
	const FPlayerRecord& Top = Potentials[0];
	for (FPlayerRecord& Record : Records)
	{
		if (Record.ID == Top.ID)
		{
			return &Record;
		}
	}
	return nullptr;
}

TArray<FPlayerRecord> UMyGameplaySystemLibrary::FindPotentials(TArray<FPlayerRecord>& Records)
{
	TArray<FPlayerRecord> Potentials;
	TArray<FPlayerRecord> RecordsTmp = Records;

	// ポイントで並べ替え(降順)
	RecordsTmp.Sort([](const FPlayerRecord& Record1, const FPlayerRecord& Record2) {
		return Record1.Point > Record2.Point;
		});

	// 一番高いポイントを持ってる記録を見つけ出す、同じの場合も出しておく
	for (const FPlayerRecord& Record : RecordsTmp)
	{
		// すでにデータが入ってる場合
		if (Potentials.Num() > 0)
		{
			// 今の記録のポイントより少ない場合
			if (Record.Point < Potentials[0].Point)
				break;
			else
				Potentials.Add(Record);
		}
		else
		{
			Potentials.Add(Record);
		}
	}

	return Potentials;
}

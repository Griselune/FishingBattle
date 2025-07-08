// Fill out your copyright notice in the Description page of Project Settings.


#include "TakimotoBranch/CPPDamageNotify.h"
#include "FishingBattle/FishingBattleCharacter.h"
#include "TakimotoBranch/CPPBaseWeapon.h"

void UCPPDamageNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	AActor* OwnerActor = MeshComp->GetOwner();

	AFishingBattleCharacter* fbc = Cast<AFishingBattleCharacter>(OwnerActor);
	if (!fbc) return;

	UChildActorComponent* WeaponChildActorComp = nullptr;

	TArray<UChildActorComponent*> ChildActorComponents;
	fbc->GetComponents<UChildActorComponent>(ChildActorComponents);

	for (UChildActorComponent* Comp : ChildActorComponents)
	{
		if (Comp && Comp->ComponentHasTag(TEXT("Weapon"))) // nullチェックとタグのチェック
		{
			WeaponChildActorComp = Comp; // タグが見つかったコンポーネントを保存
			break; // 目的のコンポーネントが見つかったのでループを抜ける
		}
	}
	if (!WeaponChildActorComp) return;

	AActor* ChildActor = WeaponChildActorComp->GetChildActor();
	if (!ChildActor) return;

	ACPPBaseWeapon* Weapon = Cast<ACPPBaseWeapon>(ChildActor);
	if (Weapon)
	{
		Weapon->Attack_Begin(); // 仮の関数名
	}
	else
	{
		return;
	}
}

void UCPPDamageNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	AActor* OwnerActor = MeshComp->GetOwner();

	AFishingBattleCharacter* fbc = Cast<AFishingBattleCharacter>(OwnerActor);
	if (!fbc) return;

	UChildActorComponent* WeaponChildActorComp = nullptr;

	TArray<UChildActorComponent*> ChildActorComponents;
	fbc->GetComponents<UChildActorComponent>(ChildActorComponents);

	for (UChildActorComponent* Comp : ChildActorComponents)
	{
		if (Comp && Comp->ComponentHasTag(TEXT("Weapon"))) // nullチェックとタグのチェック
		{
			WeaponChildActorComp = Comp; // タグが見つかったコンポーネントを保存
			break; // 目的のコンポーネントが見つかったのでループを抜ける
		}
	}
	if (!WeaponChildActorComp) return;

	AActor* ChildActor = WeaponChildActorComp->GetChildActor();
	if (!ChildActor) return;

	ACPPBaseWeapon* Weapon = Cast<ACPPBaseWeapon>(ChildActor);
	if (Weapon)
	{
		Weapon->Attack_Finish(); // 仮の関数名
	}
	else
	{
		return;
	}
}
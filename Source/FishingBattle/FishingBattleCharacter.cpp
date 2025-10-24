// Copyright Epic Games, Inc. All Rights Reserved.

#include "FishingBattleCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "tokumaru/PlayerState_T.h"
#include "InputActionValue.h"
#include <Kismet/GameplayStatics.h>
#include <WuBranch/Actor/FishingGround.h>
#include <WuBranch/PlayerController/FisherController.h>
#include <PrinzBranch/LANGameInstance.h>
#include "Components/WidgetComponent.h" //プリンス 追加 2025/10/21　ネームタグに使う
#include "WuBranch/Interface/NameUI.h" //プリンス 追加 2025/10/21　ネームタグに使う
#include "PrinzBranch/ChildPlayerNameTag.h" //プリンス 追加 2025/10/24　ネームタグに使う 
#include "Components/TextBlock.h" //プリンス 追加 2025/10/24　ネームタグに使う 

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFishingBattleCharacter

AFishingBattleCharacter::AFishingBattleCharacter()
	: Sea(nullptr)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	effect->SetupAttachment(RootComponent);

	damageEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DamageEffect"));
	damageEffect->SetupAttachment(RootComponent);


	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//プリンス START 2025/10/21
//	NameTagWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameTagWidget"));
//	NameTagWidgetComp->SetupAttachment(RootComponent);
//	NameTagWidgetComp->SetDrawAtDesiredSize(true);
//	NameTagWidgetComp->SetTwoSided(true);
//	NameTagWidgetComp->SetVisibility(true);

	// Create the child actor component
	ChildActorComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActorComponent"));
	// Set it as the root component, or attach it to another scene component
	SetRootComponent(ChildActorComp);



	bReplicates = true;
	bAlwaysRelevant = true; // Optional, ensures all players see all characters

	Crown = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crown"));
	Crown->SetupAttachment(RootComponent);
	//プリンス END 2025/10/21

	this->Tags.Add(FName("Player"));
}

float AFishingBattleCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 2025.07.24 ウー start
	if (!HasAuthority())
	{
		Server_DamageEffect();
		return 0.0f;
	}
	// 2025.07.24 ウー end
	// 回避状態に入ってる
	if (IsRoll)return 0.0f;
	// 既に死んだら
	if (IsDead)return 0.0f;
	if (UnDead)return 0.0f;

	// 2025.07.24 ウー start
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	this->Health -= Damage;

	// 自分も更新する, Replicatedはサーバー自身のReplicatedUsingにバインドされた関数を呼ばない
	UpdateHP(MaxHealth, Health);
	// 2025.07.24 ウー end

	if (Health <= 0.0f) {
		Die();

		// 2025.10.23 ウー start
		// ポイントを加算
		if (AFishingBattleCharacter* Player = Cast<AFishingBattleCharacter>(DamageCauser))
		{
			Player->AddPoint(AttackPoint);
		}
		// 2025.10.23 ウー end

		//if (HasAuthority()) {
		//	Multi_Dead();
		//}
		//else {
		//	Server_Dead();
		//}
	}

	Multi_DamageEffect();
	return DamageAmount;
}




void AFishingBattleCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("initHP %f"), Health);

	

	// 2025.07.24 ウー start
	//healthUpdate.AddDynamic(this, &AFishingBattleCharacter::GetPlayerHealth);
	Health = MaxHealth;

	// サーバーのみ、
	if (HasAuthority())
		SetNameFromInstance();
	// 2025.07.24 ウー end

	//スポーンしてから5秒無敵にする
	FTimerHandle WeaponCheckTimer;
	GetWorldTimerManager().SetTimer(
		WeaponCheckTimer,
		FTimerDelegate::CreateLambda([this]() {UnDead = false;}),
		5.0f,
		false
	);

	//プリンス　START 2025/10/22
// For the local and remote clients

	if (IsLocallyControlled())
	{
		ULANGameInstance* GameInstance = GetGameInstance<ULANGameInstance>();
		if (GameInstance)
		{
			ServerSetPlayerName(GameInstance->GIPlayerName);
		}
	}
	//if (NameTagWidgetComp)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("WidgetComponent exists, visibility: %d, space: %d, hasWidget: %d"),
	//		NameTagWidgetComp->IsVisible(),
	//		(int32)NameTagWidgetComp->GetWidgetSpace(),
	//		NameTagWidgetComp->GetWidget() != nullptr);
	//	// Delay until widget exists
	//	FTimerHandle WidgetCheckTimer;
	//	GetWorldTimerManager().SetTimer(
	//		WidgetCheckTimer,
	//		FTimerDelegate::CreateLambda([this]()
	//			{
	//				if (UUserWidget* Widget = NameTagWidgetComp->GetWidget())
	//				{
	//					UE_LOG(LogTemp, Warning, TEXT("✅ Widget created - updating name"));
	//					UpdateNameWidget();
	//				}
	//				else
	//				{
	//					UE_LOG(LogTemp, Warning, TEXT("⏳ Widget not ready yet"));
	//				}
	//			}),
	//		0.2f,   // small delay
	//		false
	//	);
	//}

	/*FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
		{
			if (NameTagWidgetComp)
			{
				NameTagWidgetComp->InitWidget();
				NameTagWidgetComp->SetVisibility(true);
				NameTagWidgetComp->SetTwoSided(true);
				UE_LOG(LogTemp, Warning, TEXT("✅ NameTagWidget manually initialized"));
			}
		}, 0.5f, false);*/
	//プリンス END 2025/10/22
}

#pragma region RPC関数

void AFishingBattleCharacter::Server_Dead_Implementation()
{
	Multi_Dead();
}

void AFishingBattleCharacter::Multi_Dead_Implementation()
{
	if (IsDead)return;
	UE_LOG(LogTemp, Warning, TEXT("dead!start"));
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance) {
		animInstance->Montage_Play(DeadMontage);
		IsDead = true;
	}

	FOnMontageEnded Delegate;
	Delegate.BindUObject(this, &AFishingBattleCharacter::OnDeadEnded);
	animInstance->Montage_SetEndDelegate(Delegate, DeadMontage);

	//カウンター加算
	AddToDeadCounter();

	//マッピングコンテクストの変更で操作を制限
	ChangeMappingContext(DeadMappingContext);
}


void AFishingBattleCharacter::Server_Die_Implementation()
{
	Multi_Die();
}

void AFishingBattleCharacter::Multi_Die_Implementation()
{
	HandleDeath();
}

void AFishingBattleCharacter::Multi_Attack_Implementation()
{
	if (IsPlayAttack1 || !AttackMontage || IsRoll) return;
	UE_LOG(LogTemp, Warning, TEXT("attack!"));

	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance)return;
	UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
	if (animInstance && mAnim) {
		//ジャンプ攻撃をできるようにする。
		//if (mAnim->Isjump) return;
		FOnMontageEnded Delegate;
		if (weaponActor) {
			if (ACPPBaseWeapon* bw = Cast<ACPPBaseWeapon>(weaponActor)) {
				switch (this->WeaponType) {
				case ECPPWeaponType::WeaponTest3:

					mAnim->attack1 = true;
					IsPlayAttack1 = true;
					animInstance->Montage_Play(HeavyAttackMontage, 1.0f);

					Delegate.BindUObject(this, &AFishingBattleCharacter::OnAttackEnded);
					animInstance->Montage_SetEndDelegate(Delegate, HeavyAttackMontage);
					break;
				case ECPPWeaponType::WeaponTest6:
					mAnim->attack1 = true;
					IsPlayAttack1 = true;
					animInstance->Montage_Play(HeavyAttackMontage, 1.0f);

					Delegate.BindUObject(this, &AFishingBattleCharacter::OnAttackEnded);
					animInstance->Montage_SetEndDelegate(Delegate, HeavyAttackMontage);
					break;
				default:

					mAnim->attack1 = true;
					IsPlayAttack1 = true;
					animInstance->Montage_Play(AttackMontage, bw->AttackSpeed);

					Delegate.BindUObject(this, &AFishingBattleCharacter::OnAttackEnded);
					animInstance->Montage_SetEndDelegate(Delegate, AttackMontage);
					break;
				}
			}
		}
	}
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	if (ps) {
		if (ps->InGamePlay) {
			UE_LOG(LogTemp, Warning, TEXT("InGamePlay True"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("InGamePlay False"));
		}
	}
}

void AFishingBattleCharacter::Multi_Roll_Implementation()
{
	if (IsRoll || !RollMontage || IsPlayAttack1 || !GetCharacterMovement()->IsMovingOnGround()) return;
	UE_LOG(LogTemp, Warning, TEXT("Roll!"));

	if (!CanRoll)return;

	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance)return;
	UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
	if (animInstance && mAnim) {
		if (mAnim->Isjump) return;
		//GetCharacterMovement()->DisableMovement();
		animInstance->Montage_Play(RollMontage);
		IsRoll = true;

		CanRoll = false;
		APlayerState_T* ps = GetPlayerState<APlayerState_T>();
		if (ps) {
			ps->CanRollInPS = CanRoll;
		}


		FOnMontageEnded Delegate;
		Delegate.BindUObject(this, &AFishingBattleCharacter::OnRollEnded);
		animInstance->Montage_SetEndDelegate(Delegate, RollMontage);
	}
}

void AFishingBattleCharacter::Multi_Fishing_Implementation()
{
	// 向きの調整
	if (AFishingGround* Fishing = Cast<AFishingGround>(fishingSpot)) {
		FVector Point = Fishing->GetFishingPointOnSea();
		FRotator angle = (Point - GetActorLocation()).Rotation();
		SetActorRotation(angle);
	}

	EquipFishlot();

	//アニメーション
	if (IsRoll || !FishingMontage || IsPlayAttack1 || !GetCharacterMovement()->IsMovingOnGround())return;
	UE_LOG(LogTemp, Warning, TEXT("Fishing!"));
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance)return;
	UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
	if (!mAnim)return;
	if (!IsFishing) {
		if (mAnim->Isjump) return;
		animInstance->Montage_Play(FishingMontage);
		mAnim->IsFishing = true;
		UE_LOG(LogTemp, Warning, TEXT("Fishing!Play"));
		IsFishing = true;
		OnRep_IsFishing();

		//FOnMontageEnded Delegate;
		//Delegate.BindUObject(this, &AFishingBattleCharacter::ShowFishingGaugeNext);


		// 2025.10.21 ウー start
		//if (IsLocallyControlled())
		//{
			FOnMontageEnded Delegate;
			Delegate.BindUObject(this, &AFishingBattleCharacter::ShowFishingGauge);
			animInstance->Montage_SetEndDelegate(Delegate, FishingMontage);

			ChangeMappingContext(FishingMappingContext);
		//}
		// 2025.10.21 ウー end
	}
}

void AFishingBattleCharacter::Server_Attack_Implementation()
{
	Multi_Attack();
}

void AFishingBattleCharacter::Server_Roll_Implementation()
{
	Multi_Roll();
}

void AFishingBattleCharacter::Server_Fishing_Implementation()
{
	Multi_Fishing();
}

void  AFishingBattleCharacter::Multi_AddWeaponInPlayer_Implementation(TSubclassOf<AActor> WeaponID) {
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	//TSubclassOf<AActor> weaponActorSubclass = Cast<AFishingGround>(fishingSpot)->GetFish();
	if (ps)
	{
		if (WeaponID) {
			UE_LOG(LogTemp, Error, TEXT("addweaponInPlayer!!!!!!!!!!!!!!!!!!!!!!!"));
			ps->Server_AddWeapon(WeaponID);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("プレイヤーステートないけどどうする？"));
	}
}

void AFishingBattleCharacter::Server_EquipWeapon_Implementation(TSubclassOf<AActor> weaponID) {
	//EquipWeapon(weaponID);
	Multi_EquipWeapon(weaponID);
}

void AFishingBattleCharacter::Multi_EquipWeapon_Implementation(TSubclassOf<AActor> weaponID) {
	EquipWeapon(weaponID);
}

void AFishingBattleCharacter::Server_EquipSlotIndex_Implementation(int slotIndex) {
	//EquipSlotIndex(slotIndex);
	Multi_EquipSlotIndex(slotIndex);
}

void AFishingBattleCharacter::Multi_EquipSlotIndex_Implementation(int slotIndex) {
	EquipSlotIndex(slotIndex);
}

void AFishingBattleCharacter::Server_TrueInGamePlay_Implementation()
{
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	if (ps) {
		ps->InGamePlay = true;
	}
}

void AFishingBattleCharacter::Server_BeginAddFishrot_Implementation() {
	//Multi_BeginAddFishrot();
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	if (ps)
	{
		UE_LOG(LogTemp, Error, TEXT("addweaponInPlayer!!!!!!!!!!!!!!!!!!!!!!!"));
		ps->Server_AddWeapon(FishRod);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("プレイヤーステートないけどどうする？"));
	}
}

void AFishingBattleCharacter::Multi_BeginAddFishrot_Implementation()
{
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	if (ps)
	{
		UE_LOG(LogTemp, Error, TEXT("addweaponInPlayer!!!!!!!!!!!!!!!!!!!!!!!"));
		ps->Server_AddWeapon(FishRod);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("プレイヤーステートないけどどうする？"));
	}
}

void AFishingBattleCharacter::Server_DestructionWeapon_Implementation(int index) {
	Multi_DestructionWeapon(index);
}

void AFishingBattleCharacter::Multi_DestructionWeapon_Implementation(int index) {
	if (IsPlayAttack1)return;
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	if (!ps)return;
	ps->Server_DestructionWeaponPS(index);

	if (HasAuthority()) {
		if (ACPPBaseWeapon* baseWeapon = Cast<ACPPBaseWeapon>(weaponActor)) {
			Heal(baseWeapon->HealingAmount, 0);
		}
	}

	EquipSlotIndex(index);
}

//10月15日　滝本海大　開始
void AFishingBattleCharacter::Server_GetFishByGauge_Implementation(bool Result)
{
	Multi_GetFishByGauge(Result);
}

void AFishingBattleCharacter::Multi_GetFishByGauge_Implementation(bool Result)
{
	UE_LOG(LogTemp, Display, TEXT("Multi_GetFishByGauge inside"));

	//UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	//if (!animInstance)return;
	//UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
	//if (!mAnim)return;
	//mAnim->IsFishing = false;
	//animInstance->Montage_Play(UpLot);
	OnFishingEnded(Result);

}
//10月15日　滝本海大　終了

void AFishingBattleCharacter::Server_EnterSpot_Implementation(AActor* spot)
{
	Multi_EnterSpot(spot);
}

void AFishingBattleCharacter::Multi_EnterSpot_Implementation(AActor* spot)
{
	canFishing = true;
	if (spot)
	{
		fishingSpot = spot;
	}
}

void AFishingBattleCharacter::Server_ExitSpot_Implementation()
{
	Multi_ExitSpot();
}

void AFishingBattleCharacter::Multi_ExitSpot_Implementation()
{
	canFishing = false;
	fishingSpot = nullptr;
}

#pragma endregion

#pragma region 武器装備
void AFishingBattleCharacter::EquipSlotIndex(int slotIndex)
{
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	if (!ps)return;


	//引数の番号を参照してプレイヤーステート内のインベントリから武器を取得
	//インベントリに武器がなければ素手になる。
	const FInventoryWeapon* weapon = ps->GetweaponSlot(slotIndex);
	if (weapon) {
		if (weapon->weaponActor) {
			Server_EquipWeapon(weapon->weaponActor);
			nowInventoryIndex = slotIndex;
		}
		else {
			if (weaponActor) {
				weaponActor->Destroy();
			}

			weaponActor = nullptr;

			FTimerHandle WeaponCheckTimer;
			GetWorld()->GetTimerManager().SetTimer(
				WeaponCheckTimer,
				this,
				&AFishingBattleCharacter::DelayedCheckWeaponType,
				0.1f, // 100ms の遅延
				false
			);
			nowInventoryIndex = slotIndex;
		}
	}
	else {

		if (weaponActor) {
			weaponActor->Destroy();
		}

		weaponActor = nullptr;

		FTimerHandle WeaponCheckTimer;
		GetWorld()->GetTimerManager().SetTimer(
			WeaponCheckTimer,
			this,
			&AFishingBattleCharacter::DelayedCheckWeaponType,
			0.1f, // 100ms の遅延
			false
		);
		nowInventoryIndex = slotIndex;
	}
}

void AFishingBattleCharacter::DestructionWeapon()
{
	if (!HasAuthority())
	{
		Server_DestructionWeapon(nowInventoryIndex);
	}
	else {
		Multi_DestructionWeapon(nowInventoryIndex);
	}
}



void AFishingBattleCharacter::EquipSlot1()
{
	if (IsPlayAttack1)return;
	UE_LOG(LogTemp, Warning, TEXT("weapon1"));
	//EquipSlotIndex(1);
	if (!HasAuthority())
	{
		Server_EquipSlotIndex(1);
	}
	else {
		Multi_EquipSlotIndex(1);
	}
	//ChangeMappingContext(HasweaponMappingContext);
	//サーバー同期の関係か、変数の取得を直接行うと値が反映されないので、
	//少し遅延をかけて取得できるようにしている。(0.01秒)
	FTimerHandle WeaponCheckTimer;
	GetWorldTimerManager().SetTimer(
		WeaponCheckTimer,
		FTimerDelegate::CreateLambda([this]() {
			if (weaponActor) {
				ChangeMappingContext(HasweaponMappingContext);
			}
			else {
				ChangeMappingContext(DefaultMappingContext);
			}
			}),
		0.1f,
		false
	);
}

void AFishingBattleCharacter::EquipSlot2()
{
	if (IsPlayAttack1)return;
	UE_LOG(LogTemp, Warning, TEXT("weapon2"));
	//EquipSlotIndex(2);
	if (!HasAuthority())
	{
		Server_EquipSlotIndex(2);
	}
	else {
		Multi_EquipSlotIndex(2);
	}
	//ChangeMappingContext(HasweaponMappingContext);
	//サーバー同期の関係か、変数の取得を直接行うと値が反映されないので、
	//少し遅延をかけて取得できるようにしている。(0.01秒)
	FTimerHandle WeaponCheckTimer;
	GetWorldTimerManager().SetTimer(
		WeaponCheckTimer,
		FTimerDelegate::CreateLambda([this]() {
			if (weaponActor) {
				ChangeMappingContext(HasweaponMappingContext);
			}
			else {
				ChangeMappingContext(DefaultMappingContext);
			}
			}),
		0.1f,
		false
	);
}

void AFishingBattleCharacter::EquipSlot3()
{
	if (IsPlayAttack1)return;
	UE_LOG(LogTemp, Warning, TEXT("weapon3"));
	//EquipSlotIndex(3);
	if (!HasAuthority())
	{
		Server_EquipSlotIndex(3);
	}
	else {
		Multi_EquipSlotIndex(3);
	}
	//ChangeMappingContext(HasweaponMappingContext);
	//サーバー同期の関係か、変数の取得を直接行うと値が反映されないので、
	//少し遅延をかけて取得できるようにしている。(0.01秒)
	FTimerHandle WeaponCheckTimer;
	GetWorldTimerManager().SetTimer(
		WeaponCheckTimer,
		FTimerDelegate::CreateLambda([this]() {
			if (weaponActor) {
				ChangeMappingContext(HasweaponMappingContext);
			}
			else {
				ChangeMappingContext(DefaultMappingContext);
			}
			}),
		0.1f,
		false
	);
}

void AFishingBattleCharacter::EquipFishlot()
{
	if (IsPlayAttack1)return;
	UE_LOG(LogTemp, Warning, TEXT("fishing"));
	//EquipSlotIndex(0);
	if (!HasAuthority())
	{
		Server_EquipSlotIndex(0);
	}
	else {
		Multi_EquipSlotIndex(0);
	}
	//サーバー同期の関係か、変数の取得を直接行うと値が反映されないので、
    //少し遅延をかけて取得できるようにしている。(0.01秒)
	FTimerHandle WeaponCheckTimer;
	GetWorldTimerManager().SetTimer(
		WeaponCheckTimer,
		FTimerDelegate::CreateLambda([this]() {
			if (weaponActor) {
				ChangeMappingContext(HasFishrotMappingContext);
			}
			else {
				ChangeMappingContext(DefaultMappingContext);
			}
			}),
		1.0f,
		false
	);
	//ChangeMappingContext(HasFishrotMappingContext);
}

void AFishingBattleCharacter::EquipWeapon(TSubclassOf<AActor> weaponID)
{
	//すでに武器を持っていたら破壊。
	if (weaponActor) {
		weaponActor->Destroy();
		weaponActor = nullptr;
	}


	//アクターをスポーンしてソケットにアタッチする方法
	//AGameMode_T* gm = Cast<AGameMode_T>(UGameplayStatics::GetGameMode(this));
	//if (!gm)return;

	//TSubclassOf<AActor> weaponClass = gm->GetWeaponClass(weaponID);
	//if (weaponClass) {
	//	FActorSpawnParameters spawnParams;
	//	spawnParams.Owner = this;
	//AActor* newWeapon = GetWorld()->SpawnActor<AActor>(weaponClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);

	//		//weaponActor = newWeapon;
	//		//weaponActor->SetOwner(this);
	//		//weaponActor->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("wepon"));
	//	}
	//}

	//-----------------------------------------------------------------------

	//アクターをチャイルドアクターに登録する方法
	AGameMode_T* gm = Cast<AGameMode_T>(UGameplayStatics::GetGameMode(this));
	if (!gm)return;

	UE_LOG(LogTemp, Warning, TEXT("weaponClass is: %s"), *weaponID->GetName());
	//if (weaponClass) {

	//ポーンのチャイルドアクターコンポーネント取得
	UChildActorComponent* weaponChildComponent = nullptr;
	TArray<UChildActorComponent*> ChildActorComponents;
	GetComponents<UChildActorComponent>(ChildActorComponents);
	for (UChildActorComponent* comp : ChildActorComponents)
	{
		if (comp->GetName() == TEXT("wepon"))
		{
			weaponChildComponent = comp;
			break;
		}
	}
	//チャイルドアクターコンポーネントにアクターをアタッチ、現在装備している武器として登録
	if (weaponChildComponent)
	{
		if (weaponChildComponent->GetChildActor())
		{
			weaponChildComponent->GetChildActor()->Destroy();
			weaponChildComponent->SetChildActorClass(nullptr); // 念のため明示的に外す
		}
		weaponChildComponent->SetChildActorClass(weaponID); // TSubclassOf<AActor>

		weaponActor = weaponChildComponent->GetChildActor();
		if (weaponActor)
		{
			UE_LOG(LogTemp, Error, TEXT("setowner!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
			weaponActor->SetOwner(this);
		}
	}

	FTimerHandle WeaponCheckTimer;
	GetWorld()->GetTimerManager().SetTimer(
		WeaponCheckTimer,
		this,
		&AFishingBattleCharacter::DelayedCheckWeaponType,
		0.1f, // 100ms の遅延
		false
	);
}

void AFishingBattleCharacter::DelayedCheckWeaponType()
{
	//武器のタイプを取得
	//ACPPBaseWeapon* Base = Cast<ACPPBaseWeapon>(weaponActor);
	if (ACPPBaseWeapon* Base = Cast<ACPPBaseWeapon>(weaponActor))
	{
		UE_LOG(LogTemp, Error, TEXT("GetWeaponTypeInCharacter!!!!!!"));
		this->WeaponType = Base->WeaponType;
		OnRep_AnimInstance();

	}
	else {
		this->WeaponType = ECPPWeaponType::None;
		OnRep_AnimInstance();
	}
}

#pragma endregion

#pragma region Valiable
bool AFishingBattleCharacter::Server_Dead_Validate() {
	return true;
}

bool AFishingBattleCharacter::Multi_Dead_Validate() {
	return true;
}

bool AFishingBattleCharacter::Server_Attack_Validate() {
	return true;
}

bool AFishingBattleCharacter::Server_Roll_Validate() {
	return true;
}

bool AFishingBattleCharacter::Server_Fishing_Validate() {
	return true;
}

bool AFishingBattleCharacter::Multi_Attack_Validate() {
	return true;
}

bool AFishingBattleCharacter::Multi_Roll_Validate() {
	return true;
}

bool AFishingBattleCharacter::Multi_Fishing_Validate() {
	return true;
}

bool AFishingBattleCharacter::Server_BeginAddFishrot_Validate() {
	return true;
}

bool AFishingBattleCharacter::Server_EquipWeapon_Validate(TSubclassOf<AActor> weaponID) {
	return true;
}

bool AFishingBattleCharacter::Multi_EquipWeapon_Validate(TSubclassOf<AActor> weaponID) {
	return true;
}

bool AFishingBattleCharacter::Server_EquipSlotIndex_Validate(int slotIndex) {
	return true;
}

bool AFishingBattleCharacter::Multi_EquipSlotIndex_Validate(int slotIndex) {
	return true;
}

bool AFishingBattleCharacter::Multi_AddWeaponInPlayer_Validate(TSubclassOf<AActor> WeaponID) {
	return true;
}

bool AFishingBattleCharacter::Server_Die_Validate() {
	return true;
}

bool AFishingBattleCharacter::Multi_Die_Validate() {
	return true;
}
bool AFishingBattleCharacter::Server_TrueInGamePlay_Validate() {
	return true;
}

bool AFishingBattleCharacter::Multi_BeginAddFishrot_Validate() {
	return true;
}

bool AFishingBattleCharacter::Server_DestructionWeapon_Validate(int index) {
	return true;
}

bool AFishingBattleCharacter::Multi_DestructionWeapon_Validate(int index) {
	return true;
}
#pragma endregion

#pragma region オーバーライド

//プレイヤーステート等の初期化が終わってから呼ばれる
void AFishingBattleCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			nowMappingContext = DefaultMappingContext;
			//Subsystem->AddMappingContext(FishingMappingContext, 0);
			//Subsystem->RemoveMappingContext(DefaultMappingContext);
		}
	}
}

//レプリケートする変数の登録
void AFishingBattleCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFishingBattleCharacter, weaponActor);
	DOREPLIFETIME(AFishingBattleCharacter, WeaponType);
	DOREPLIFETIME(AFishingBattleCharacter, IsFishing);
	DOREPLIFETIME(AFishingBattleCharacter, weaponActorSubclass);
	DOREPLIFETIME(AFishingBattleCharacter, fishingSpot);
	// 2025.07.24 ウー start
	DOREPLIFETIME(AFishingBattleCharacter, Health);
	// 2025.07.24 ウー end
	// 
	//プリンス START 2025/10/21
	DOREPLIFETIME(AFishingBattleCharacter, Name);
//	DOREPLIFETIME_CONDITION_NOTIFY(AFishingBattleCharacter, Name, COND_None, REPNOTIFY_Always);
	//プリンス END 2025/10/21
}

//プレイヤーがコントローラーを取得したときに呼ばれる
void AFishingBattleCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (HasAuthority()) {
		Multi_BeginAddFishrot();
		UE_LOG(LogTemp, Warning, TEXT("addFishrod!!!!!!!!!!!!"));

		Server_TrueInGamePlay();
	}

	//プリンス START 2025/10/22
//	if (HasAuthority())
//	{
//		if (IsLocallyControlled())
//		{
			ULANGameInstance* GameInstance = GetGameInstance<ULANGameInstance>();
			if (GameInstance)
			{
				ServerSetPlayerName(GameInstance->GIPlayerName);
			}
//		}
//	}
	//プリンス END 2025/10/22

}

void AFishingBattleCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 名前を保存する、クライアントのみ
	SetNameFromInstance();

}

#pragma endregion

#pragma region Input
void AFishingBattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFishingBattleCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFishingBattleCharacter::Look);

		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AFishingBattleCharacter::Attack1);

		//Roll
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &AFishingBattleCharacter::Roll);

		//Fishing
		EnhancedInputComponent->BindAction(FishingAction, ETriggerEvent::Started, this, &AFishingBattleCharacter::Fishing);

		EnhancedInputComponent->BindAction(SwitchWeapon1, ETriggerEvent::Started, this, &AFishingBattleCharacter::EquipSlot1);

		EnhancedInputComponent->BindAction(SwitchWeapon2, ETriggerEvent::Started, this, &AFishingBattleCharacter::EquipSlot2);

		EnhancedInputComponent->BindAction(SwitchWeapon3, ETriggerEvent::Started, this, &AFishingBattleCharacter::EquipSlot3);

		EnhancedInputComponent->BindAction(SwitchFishlot, ETriggerEvent::Started, this, &AFishingBattleCharacter::EquipFishlot);

		EnhancedInputComponent->BindAction(DestructionWeaponInput, ETriggerEvent::Started, this, &AFishingBattleCharacter::DestructionWeapon);

		EnhancedInputComponent->BindAction(GaugeStop, ETriggerEvent::Started, this, &AFishingBattleCharacter::OnGaugeStop);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFishingBattleCharacter::ChangeMappingContext(UInputMappingContext* context_)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			//現在のマッピングコンテクストを削除、あたらしいものにきりかえる。
			Subsystem->RemoveMappingContext(nowMappingContext);
			Subsystem->AddMappingContext(context_, 0);
			nowMappingContext = context_;
		}
	}
}
#pragma endregion

#pragma region 外部呼出し
// 2025.07.17 ウー start
void AFishingBattleCharacter::Heal_Implementation(float healAmount, float Point)
{
	// サーバー側のみ実行する
	if (!HasAuthority())
		return;

	if (IsDead)
		return;

	Health += healAmount;
	if (Health > MaxHealth) {
		Health = MaxHealth;
	}
	//回復エフェクト再生
	Multi_Heal();
	// 自分も更新する
	UpdateHP(MaxHealth, Health);
	// ポイント
	AddPoint(Point);
}

void AFishingBattleCharacter::Multi_Heal_Implementation()
{
	//effect->Deactivate();
	//effect->Activate();
	effect->ActivateSystem();
}

void AFishingBattleCharacter::Server_Heal_Implementation()
{
	Multi_Heal();
}

void AFishingBattleCharacter::Multi_DamageEffect_Implementation()
{
	damageEffect->ActivateSystem();
}

void AFishingBattleCharacter::Server_DamageEffect_Implementation()
{
	Multi_DamageEffect();
}

void AFishingBattleCharacter::EnterSea(AActor* Actor)
{
	Sea = Actor;
}

float AFishingBattleCharacter::GetMaxHealth() const
{
	return MaxHealth;
}
// 2025.07.17 ウー end

void AFishingBattleCharacter::EnterSpot(AActor* spot)
{
	if (HasAuthority()) {
		Multi_EnterSpot(spot);
	}
	else {
		Server_EnterSpot(spot);
	}
	//canFishing = true;
	//if (spot)
	//{
	//	fishingSpot = spot;
	//}
}

void AFishingBattleCharacter::ExitSpot()
{
	if (HasAuthority()) {
		Multi_ExitSpot();
	}
	else{
		Server_ExitSpot();
	}
	//canFishing = false;
	//fishingSpot = nullptr;
}
#pragma endregion

#pragma region レプリケートRep

void AFishingBattleCharacter::OnRep_EquipWeapon()
{
	if (weaponActor) {
		weaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("wepon"));
	}
}

void AFishingBattleCharacter::OnRep_AnimInstance()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance)return;
	UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
	if (!mAnim)return;
	mAnim->GetWeaponType(this->WeaponType);
}

void AFishingBattleCharacter::OnRep_FishingSpot()
{
	return;
}

void AFishingBattleCharacter::OnRep_IsFishing()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance)return;
	UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
	if (!mAnim)return;
	mAnim->IsFishing = this->IsFishing;
}

void AFishingBattleCharacter::OnRep_WeaponActorSubclass()
{
	return;
}
#pragma endregion

#pragma region 体力UIで使用
// 2025.07.24 ウー start

void AFishingBattleCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	UpdateHP(MaxHealth, Health);
}

void AFishingBattleCharacter::UpdateHP(float MaxHP, float NewHP)
{
	if (AFisherController* PC = Cast<AFisherController>(GetController()))
	{
		PC->UpdateHP(MaxHP, NewHP);
	}
}

void AFishingBattleCharacter::OnRep_UpdatedHealth()
{
	UpdateHP(MaxHealth, Health);
}
// 2025.07.24 ウー end
#pragma endregion 

#pragma region リスポーン
void AFishingBattleCharacter::HandleDeath()
{
	// リスポーン要求
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	if (ps) {
		ps->InventoryInitialize();
		if (weaponActor) {
			weaponActor->Destroy();
		}
	}

	FTimerHandle RespawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		RespawnTimerHandle,
		this,
		&AFishingBattleCharacter::RequestRespawn,
		2.0f,
		false
	);
}

void AFishingBattleCharacter::RequestRespawn()
{
	UE_LOG(LogTemp, Warning, TEXT("リクエストリスポーン"));

	AController* MyController = GetController();
	if (MyController)
	{
		if (AGameMode_T* GM = Cast<AGameMode_T>(UGameplayStatics::GetGameMode(this)))
		{
			GM->RespawnPlayerT(MyController);
		}
	}

	Destroy(); // 消滅
}
#pragma endregion

// 2025.07.24 ウー start
#pragma region データ設定
void AFishingBattleCharacter::SetNameFromInstance()
{
	if (IsLocallyControlled())
	{
		ULANGameInstance* GameInstance = GetGameInstance<ULANGameInstance>();
		APlayerState_T* GameState = GetPlayerState<APlayerState_T>();
		if (GameInstance && GameState)
		{
			GameState->SetName(GameInstance->GIPlayerName);
			//prinz test
			ServerSetPlayerName(GameInstance->GIPlayerName);
		}
	}
}
#pragma endregion
// 2025.07.24 ウー end

#pragma region ステータス
void AFishingBattleCharacter::AddToDeadCounter()
{
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	if (ps) {
		ps->DeadCounter++;
		UE_LOG(LogTemp, Warning, TEXT("DeadCounter %d"), ps->DeadCounter);
	}
}
#pragma endregion

#pragma region アニメーションモンタージュ再生から終了までの処理(キー入力にバインド)
void AFishingBattleCharacter::Move(const FInputActionValue& Value)
{
	if (IsFishing)return;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFishingBattleCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFishingBattleCharacter::Attack1()
{
	if (!HasAuthority()) {
		Server_Attack();
		return;
	}
	else {
		Multi_Attack();
		return;
	}

}

void AFishingBattleCharacter::OnAttackEnded(UAnimMontage* Montage, bool in) {
	UE_LOG(LogTemp, Warning, TEXT("attack!end"));
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance)return;
	UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
	if (!mAnim)return;



	IsPlayAttack1 = false;
	mAnim->attack1 = false;
}

void AFishingBattleCharacter::Roll()
{

	if (!HasAuthority())
	{
		Server_Roll();
		return;
	}
	else {
		Multi_Roll();
		return;
	}
}

void AFishingBattleCharacter::ReloadCanRoll()
{
	CanRoll = true;
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	if (ps) {
		ps->CanRollInPS = CanRoll;
	}
}

void AFishingBattleCharacter::OnRollEnded(UAnimMontage* Montage, bool in)
{
	UE_LOG(LogTemp, Warning, TEXT("Roll!end"));
	IsRoll = false;

	// 海にいるなら
	if (Sea)
	{
		UGameplayStatics::ApplyDamage(this, GetMaxHealth(), nullptr, Sea, UDamageType::StaticClass());
	}

	FTimerHandle ReloadRoll;
	GetWorld()->GetTimerManager().SetTimer(
		ReloadRoll,
		this,
		&AFishingBattleCharacter::ReloadCanRoll,
		3.0f, // 100ms の遅延
		false
	);

}

void AFishingBattleCharacter::Jump()
{
	if (IsRoll)return;
	if (IsFishing)return;

	Super::Jump();
}

void AFishingBattleCharacter::Die()
{
	if (HasAuthority()) {
		Multi_Dead();
		return;
	}
	else {
		Server_Dead();
		return;
	}
}

void AFishingBattleCharacter::OnDeadEnded(UAnimMontage* Montage, bool in)
{
	UE_LOG(LogTemp, Warning, TEXT("dead!end"));
	SetActorHiddenInGame(true);
	if (!HasAuthority())
	{
		Server_Die();  // クライアントならサーバーへ要求
	}
	else {
		Multi_Die(); // サーバーならそのまま処理
	}
}




void AFishingBattleCharacter::Fishing()
{
	if (!canFishing)return;
	if (IsFishing)return;

	//動きを同期するためにサーバーに通知
	//if (!canFishing)return;
	if (!HasAuthority())
	{
		Server_Fishing();
	}
	else {
		Multi_Fishing();
	}
}

void AFishingBattleCharacter::OnFishingEnded(bool Result)
{
	UE_LOG(LogTemp, Warning, TEXT("Fishing!deligate"));

	if (!weaponActorSubclass)return;
	//weaponActorSubclassこれをローカルで更新しているので、これがtrueになるのは自分だけ。
	//マスタークライアントはサーバーを担っているため、サーバーから値が更新されている。そのためほかのクライアントからも処理が通る。
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance)return;
	UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
	if (!mAnim)return;
	mAnim->IsFishing = false;
	animInstance->Montage_Play(UpLot);


	IsFishing = false;
	OnRep_IsFishing();

	// Multi_AddWeaponInPlayerはNetMulticastを使ってるので、サーバーを含めた全員に飛ばしてる
	// 2025.10.15 ウー start
	//if (HasAuthority()) {

	if (IsLocallyControlled()) {

		if (fishingSpot)
		{
			UClass* WeaponClass = weaponActorSubclass;
			if (WeaponClass)
			{
				// 2025.10.23 ウー start
				/*if (ACPPBaseWeapon* BW = Cast<ACPPBaseWeapon>(WeaponClass->GetDefaultObject())) {
					if (Result) {
						if (true == Cast<AFisherController>(GetController())->GetStopFunction()) {
							Multi_AddWeaponInPlayer(weaponActorSubclass);
							UE_LOG(LogTemp, Display, TEXT("GetFish!!!!!!!"));
						}
						else UE_LOG(LogTemp, Error, TEXT("FishingFailed..."));
					}
				}*/
				if (Result) {
					// インベントリに追加
					//ここはIsLocallyControlledを貫通したクライアントが実行するが、ローカル処理になる
					Multi_AddWeaponInPlayer(weaponActorSubclass);
					UE_LOG(LogTemp, Display, TEXT("GetFish!!!!!!!"));
					// 釣れたので、ポイント追加
					if (ACPPBaseWeapon* BW = Cast<ACPPBaseWeapon>(WeaponClass->GetDefaultObject()))
					{
						AddPoint(BW->GetPoint());
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("FishingFailed..."));
				}
				// 2025.10.23 ウー end
			}
		}

		//マッピングコンテクストの入れ替えで操作を制限
		ChangeMappingContext(HasFishrotMappingContext);
	}
	// 2025.10.15 ウー end
	weaponActorSubclass = nullptr;

	//IsFishing = false;
	//OnRep_IsFishing();
}

void AFishingBattleCharacter::OnGaugeStop()
{
	bool Result = Cast<AFisherController>(GetController())->GetStopFunction();
	if (HasAuthority())
	{
		Multi_GetFishByGauge(Result);
	}
	else
	{
		Server_GetFishByGauge(Result);
	}
}


//void AFishingBattleCharacter::ShowFishingGaugeNext(UAnimMontage* Montage, bool in) {
//	weaponActorSubclass = Cast<AFishingGround>(fishingSpot)->GetFish();
//}

//10月15日　滝本海大　開始
void AFishingBattleCharacter::ShowFishingGauge(UAnimMontage* Montage, bool in)
{
	// 2025.10.15 ウー start
	//if (HasAuthority()) {
	if (fishingSpot) {
		if (weaponActorSubclass = Cast<AFishingGround>(fishingSpot)->GetFish()) {
			if (IsLocallyControlled()) {

				ChangeMappingContext(FishingMappingContext);
				UE_LOG(LogTemp, Display, TEXT("weaponActorSubclass: %s"), *weaponActorSubclass->GetName());
				UClass* WeaponClass = weaponActorSubclass;
				if (ACPPBaseWeapon* BW = Cast<ACPPBaseWeapon>(WeaponClass->GetDefaultObject())) {
					AFisherController* FC = Cast<AFisherController>(GetController());
					FC->ShowFishingGauge(BW->SkillCheckSpeed);
					UE_LOG(LogTemp, Display, TEXT("AFishingBattleCharacter::ShowFishingGauge() correct"));
				}

			}
		}
		else {
			UE_LOG(LogTemp, Display, TEXT("cast faild"));
		}
	}
	//if (IsLocallyControlled()) {
	//	if (fishingSpot) {
	//		if (weaponActorSubclass = Cast<AFishingGround>(fishingSpot)->GetFish()) {

	//			UE_LOG(LogTemp, Display, TEXT("weaponActorSubclass: %s"), *weaponActorSubclass->GetName());
	//			UClass* WeaponClass = weaponActorSubclass;
	//			if (ACPPBaseWeapon* BW = Cast<ACPPBaseWeapon>(WeaponClass->GetDefaultObject())) {
	//				AFisherController* FC = Cast<AFisherController>(GetController());
	//				FC->ShowFishingGauge(BW->SkillCheckSpeed);
	//				UE_LOG(LogTemp, Display, TEXT("AFishingBattleCharacter::ShowFishingGauge() correct"));
	//			}

	//		}
	//	}
	//}
	//}
	//else {
		//UE_LOG(LogTemp, Error, TEXT("MyLog| AFishingBattleCharacter::ShowFishingGauge() failed"));
	//}
	// 2025.10.15 ウー end
}
//10月15日　滝本海大　終了
#pragma endregion

// 2025.10.22 ウー start
#pragma region ポイント
void AFishingBattleCharacter::AddPoint(float Point)
{
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	if (ps) {
		ps->AddPoint(Point);
	}
}
#pragma endregion

#pragma region 名前表示（ネームタグ）
//プリンス START 2025/10/21
void AFishingBattleCharacter::SetName(const FString& NewName)
{
	Name = NewName;
	UpdateNameWidget();
}

FString AFishingBattleCharacter::GetName() const
{
	return Name;
}

void AFishingBattleCharacter::OnRep_UpdatedName()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_UpdatedName fired - new name: %s"), *Name);
	UpdateNameWidget();
}

void AFishingBattleCharacter::UpdateNameWidget()
{
	if (ChildActorComp && ChildActorComp->GetChildActor())
	{
		// Get the specific child actor instance and cast to your C++ class
		AChildPlayerNameTag* ChildNameTagActor = Cast<AChildPlayerNameTag>(ChildActorComp->GetChildActor());
		if (ChildNameTagActor)
		{
			// Access the widget component through the getter
			if (UWidgetComponent* WidgetComp = ChildNameTagActor->GetNameTagWidgetComp())
			{
				// Get the UserWidget object
				if (UUserWidget* Widget = WidgetComp->GetUserWidgetObject())
				{
					// Access the TextBlock using the meta=(BindWidget) name
					if (UTextBlock* NameTagText = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("TXT_NameTag"))))
					{
						// Set the text
						NameTagText->SetText(FText::FromString(Name));
					}
				}
			}
		}
	}



	// 名前ウィジェットを更新
	//if (UUserWidget* Widget = NameTagWidgetComp->GetWidget()) {
	//	//		NameTagWidgetComp->SetDrawAtDesiredSize(true);
	//	NameTagWidgetComp->SetVisibility(true);
	//	//		NameTagWidgetComp->InitWidget();
	//	if (Widget->Implements<UNameUI>()) {
	//		INameUI::Execute_SetName(Widget, GetName());
	//		INameUI::Execute_ShowName(Widget);
	//		if (HasAuthority()) {
	//			UE_LOG(LogTemp, Warning, TEXT("Server NameTag Set OK - %s"), *Name);
	//		}
	//		else {
	//			UE_LOG(LogTemp, Warning, TEXT("Server NameTag Set OK - %s"), *Name);
	//		}
	//	}
	//	else {
	//		UE_LOG(LogTemp, Warning, TEXT("Widget->Implements<UNameUI>() nullptr"));
	//	}

	//}
	//else {
	//	UE_LOG(LogTemp, Warning, TEXT("UUserWidget* Widget = NameTagWidgetComp->GetWidget()  nullptr"));
	//}
}

void AFishingBattleCharacter::PostNetInit()
{
	Super::PostNetInit();
	OnRep_UpdatedName(); // ensure name widget updates after replication
}

void AFishingBattleCharacter::ServerSetPlayerName_Implementation(const FString& NewName)
{
	SetName(NewName); // sets and replicates to everyone
}


//プリンス END 2025/10/21
#pragma endregion
// 2025.10.22 ウー end
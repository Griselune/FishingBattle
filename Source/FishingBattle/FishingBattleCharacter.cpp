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

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFishingBattleCharacter

AFishingBattleCharacter::AFishingBattleCharacter()
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

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

float AFishingBattleCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsRoll)return 0.0f;
	if (IsDead)return 0.0f;
	this->Health -= DamageAmount;
	if (Health <= 0.0f) {
		Die();
		//if (HasAuthority()) {
		//	Multi_Dead();
		//}
		//else {
		//	Server_Dead();
		//}
	}

	// 2025.07.17 ウー start
	BroadcastHP(100.0f, Health);
	// 2025.07.17 ウー end

	return DamageAmount;
}

// 2025.07.17 ウー start
void AFishingBattleCharacter::Heal(float healAmount)
{
	if (IsDead)
		return;

	Health += healAmount;
	if (Health > 100.0f) {
		Health = 100.0f;
	}
	BroadcastHP(100.0f, Health);
}
// 2025.07.17 ウー end

void AFishingBattleCharacter::EnterSpot(AActor* spot)
{
	canFishing = true;
	if (spot)
	{
		fishingSpot = spot;
	}
}

void AFishingBattleCharacter::ExitSpot()
{
	canFishing = false;
	fishingSpot = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Input

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

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AFishingBattleCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFishingBattleCharacter, weaponActor);
	DOREPLIFETIME(AFishingBattleCharacter, WeaponType);
}

void AFishingBattleCharacter::BeginPlay()
{
	Super::BeginPlay();

	healthUpdate.AddDynamic(this, &AFishingBattleCharacter::GetPlayerHealth);

	Server_TrueInGamePlay();


	//ゲーム開始時に武器割り当てをしようとした名残。オーナーの設定がクライアント側で間に合ってないため、プレイヤーの所有物にアクセスできない
	// サーバーのホストは可能。
	//if (HasAuthority()) {
	//	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	//	if (ps && ps->inventory.Num() == 0) {
	//		//ps->Server_AddWeapon("Fishinglot");
	//		Server_AddWeaponInPlayer("Fishinglot");
	//	}
	//}
	//else {
	//	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	//	if (ps && ps->inventory.Num() == 0) {
	//		Server_AddWeaponInPlayer("Fishinglot");
	//	}
	//}
}

void AFishingBattleCharacter::GetPlayerHealth(float maxHP, float updateHP) {
	UE_LOG(LogTemp, Warning, TEXT("イベントディスパッチャーですよ。得丸"));
}

void AFishingBattleCharacter::BroadcastHP(float maxHP, float updateHP) {
	healthUpdate.Broadcast(maxHP, updateHP);
}

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
	if (IsPlayAttack1 || !AttackMontage || IsRoll || !GetCharacterMovement()->IsMovingOnGround()) return;
	UE_LOG(LogTemp, Warning, TEXT("attack!"));

	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance)return;
	UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
	if (animInstance && mAnim) {
		if (mAnim->Isjump) return;
		animInstance->Montage_Play(AttackMontage, 2.0f);
		mAnim->attack1 = true;
		IsPlayAttack1 = true;

		FOnMontageEnded Delegate;
		Delegate.BindUObject(this, &AFishingBattleCharacter::OnAttackEnded);
		animInstance->Montage_SetEndDelegate(Delegate, AttackMontage);
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
	if (IsRoll || !RollMontage || IsPlayAttack1 || !GetCharacterMovement()->IsMovingOnGround()) return;
	UE_LOG(LogTemp, Warning, TEXT("Roll!"));

	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance)return;
	UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
	if (animInstance && mAnim) {
		if (mAnim->Isjump) return;
		//GetCharacterMovement()->DisableMovement();
		animInstance->Montage_Play(RollMontage);
		IsRoll = true;

		FOnMontageEnded Delegate;
		Delegate.BindUObject(this, &AFishingBattleCharacter::OnRollEnded);
		animInstance->Montage_SetEndDelegate(Delegate, RollMontage);
	}
}

void AFishingBattleCharacter::OnRollEnded(UAnimMontage* Montage, bool in)
{
	UE_LOG(LogTemp, Warning, TEXT("Roll!end"));
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	IsRoll = false;
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
	if (IsDead)return;
	UE_LOG(LogTemp, Warning, TEXT("dead!start"));
	GetCharacterMovement()->DisableMovement();
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance) {
		animInstance->Montage_Play(DeadMontage);
		IsDead = true;
	}

	FOnMontageEnded Delegate;
	Delegate.BindUObject(this, &AFishingBattleCharacter::OnDeadEnded);
	animInstance->Montage_SetEndDelegate(Delegate, DeadMontage);

	//FTimerHandle TimerHandle;
	//GetWorldTimerManager().SetTimer(TimerHandle, this, &AFishingBattleCharacter::canDestroy, 5.0f, false);
}

void AFishingBattleCharacter::OnDeadEnded(UAnimMontage* Montage, bool in)
{
	UE_LOG(LogTemp, Warning, TEXT("dead!end"));
	SetActorHiddenInGame(true);
	//ChangeMappingContext(DefaultMappingContext);
	if (!HasAuthority())
	{
		Server_Die();  // クライアントならサーバーへ要求
	}
	else {
		Multi_Die(); // サーバーならそのまま処理
	}
}
void AFishingBattleCharacter::Server_Dead_Implementation()
{
	Multi_Dead();
}

void AFishingBattleCharacter::Multi_Dead_Implementation()
{
	if (IsDead)return;
	UE_LOG(LogTemp, Warning, TEXT("dead!start"));
	//動きをマッピングコンテクストの変更で制限させるようにする。
	//GetCharacterMovement()->DisableMovement();
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance) {
		animInstance->Montage_Play(DeadMontage);
		IsDead = true;
	}

	FOnMontageEnded Delegate;
	Delegate.BindUObject(this, &AFishingBattleCharacter::OnDeadEnded);
	animInstance->Montage_SetEndDelegate(Delegate, DeadMontage);

	//これ
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

void AFishingBattleCharacter::Multi_Attack_Implementation()
{
	if (IsPlayAttack1 || !AttackMontage || IsRoll || !GetCharacterMovement()->IsMovingOnGround()) return;
	UE_LOG(LogTemp, Warning, TEXT("attack!"));

	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance)return;
	UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
	if (animInstance && mAnim) {
		if (mAnim->Isjump) return;
		FOnMontageEnded Delegate;
		switch (this->WeaponType) {
		case ECPPWeaponType::WeaponTest1:

			mAnim->attack1 = true;
			IsPlayAttack1 = true;
			animInstance->Montage_Play(HeavyAttackMontage, 1.0f);

			Delegate.BindUObject(this, &AFishingBattleCharacter::OnAttackEnded);
			animInstance->Montage_SetEndDelegate(Delegate, HeavyAttackMontage);
			break;
		default:

			mAnim->attack1 = true;
			IsPlayAttack1 = true;
			animInstance->Montage_Play(AttackMontage, 2.0f);

			Delegate.BindUObject(this, &AFishingBattleCharacter::OnAttackEnded);
			animInstance->Montage_SetEndDelegate(Delegate, AttackMontage);
			break;
		}
		//mAnim->attack1 = true;
		//IsPlayAttack1 = true;

		//FOnMontageEnded Delegate;
		//Delegate.BindUObject(this, &AFishingBattleCharacter::OnAttackEnded);
		//animInstance->Montage_SetEndDelegate(Delegate, AttackMontage);
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

	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (!animInstance)return;
	UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
	if (animInstance && mAnim) {
		if (mAnim->Isjump) return;
		//GetCharacterMovement()->DisableMovement();
		animInstance->Montage_Play(RollMontage);
		IsRoll = true;

		FOnMontageEnded Delegate;
		Delegate.BindUObject(this, &AFishingBattleCharacter::OnRollEnded);
		animInstance->Montage_SetEndDelegate(Delegate, RollMontage);
	}
}

void AFishingBattleCharacter::Multi_Fishing_Implementation()
{
	if (!canFishing)return;
	// 向きの調整
	if (AFishingGround* Fishing = Cast<AFishingGround>(fishingSpot)) {
		FVector Point = Fishing->GetFishingPointOnSea();
		FRotator angle = (Point - GetActorLocation()).Rotation();
		SetActorRotation(angle);
	}

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
		UE_LOG(LogTemp, Warning, TEXT("Fishing!Play"));
		IsFishing = true;

		FOnMontageEnded Delegate;
		Delegate.BindUObject(this, &AFishingBattleCharacter::OnFishingEnded);
		animInstance->Montage_SetEndDelegate(Delegate, FishingMontage);

		ChangeMappingContext(FishingMappingContext);

	}
	//釣りを途中で停止できるか否か
	//else if (IsFishing) {
	//	UE_LOG(LogTemp, Warning, TEXT("Fishing!end"));
	//	animInstance->Montage_Stop(0.2f, FishingMontage);
	//	IsFishing = false;
	//}
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

void AFishingBattleCharacter::Fishing()
{
	//if (!canFishing)return;
	if (!HasAuthority())
	{
		Server_Fishing();
		return;
	}
	else {
		Multi_Fishing();
		return;
	}
	//if (IsRoll || !FishingMontage || IsPlayAttack1 || !GetCharacterMovement()->IsMovingOnGround())return;
	//UE_LOG(LogTemp, Warning, TEXT("Fishing!"));
	//UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	//if (!animInstance)return;
	//UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
	//if (!mAnim)return;
	//if (!IsFishing) {
	//	if (mAnim->Isjump) return;
	//	animInstance->Montage_Play(FishingMontage);
	//	UE_LOG(LogTemp, Warning, TEXT("Fishing!Play"));
	//	IsFishing = true;

	//	FOnMontageEnded Delegate;
	//	Delegate.BindUObject(this, &AFishingBattleCharacter::OnFishingEnded);
	//	animInstance->Montage_SetEndDelegate(Delegate, FishingMontage);

	//}
	//else if(IsFishing){
	//	UE_LOG(LogTemp, Warning, TEXT("Fishing!end"));
	//	animInstance->Montage_Stop(0.2f,FishingMontage);
	//	IsFishing = false;
	//}
}

void AFishingBattleCharacter::OnFishingEnded(UAnimMontage* Montage, bool in)
{
	UE_LOG(LogTemp, Warning, TEXT("Fishing!deligate"));
	//APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	//if (!ps)return;
	//ps->Server_AddWeapon("Weapon1");
	//if (IsLocallyControlled()&& !HasAuthority()) {
	//	Server_AddWeaponInPlayer("Weapon1");
	//}
	//else {
	//	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	//	if (ps)
	//	{
	//		ps->Server_AddWeapon("Weapon1"); //
	//	}
	//}

	//なぜかわからないがサーバーのみで処理すると正常にクライアントも動作する
	if (HasAuthority()) {
		Server_AddWeaponInPlayer("Weapon1");
	}
	ChangeMappingContext(DefaultMappingContext);

	IsFishing = false;
}

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

bool AFishingBattleCharacter::Server_EquipWeapon_Validate(FName weaponID) {
	return true;
}

bool AFishingBattleCharacter::Multi_EquipWeapon_Validate(FName weaponID) {
	return true;
}

bool AFishingBattleCharacter::Server_EquipSlotIndex_Validate(int slotIndex) {
	return true;
}

bool AFishingBattleCharacter::Multi_EquipSlotIndex_Validate(int slotIndex) {
	return true;
}

bool AFishingBattleCharacter::Server_AddWeaponInPlayer_Validate(FName WeaponID) {
	return true;
}

bool AFishingBattleCharacter::Multi_AddWeaponInPlayer_Validate(FName WeaponID) {
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

void AFishingBattleCharacter::Server_AddWeaponInPlayer_Implementation(FName WeaponID) {
	UE_LOG(LogTemp, Error, TEXT("addweaponInPlayer!!!!!!!!!!!!!!!!!!!!!!!"));
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	if (ps)
	{
		ps->Server_AddWeapon(WeaponID); //
	}
}

void  AFishingBattleCharacter::Multi_AddWeaponInPlayer_Implementation(FName WeaponID) {
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	if (ps)
	{
		ps->Server_AddWeapon(WeaponID); //
	}
}

void AFishingBattleCharacter::Server_EquipWeapon_Implementation(FName weaponID) {
	EquipWeapon(weaponID);
	Multi_EquipWeapon(weaponID);
}

void AFishingBattleCharacter::Multi_EquipWeapon_Implementation(FName weaponID) {
	EquipWeapon(weaponID);
}

void AFishingBattleCharacter::Server_EquipSlotIndex_Implementation(int slotIndex) {
	EquipSlotIndex(slotIndex);
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

void AFishingBattleCharacter::EquipSlotIndex(int slotIndex)
{
	APlayerState_T* ps = GetPlayerState<APlayerState_T>();
	if (!ps)return;

	const FInventoryWeapon* weapon = ps->GetweaponSlot(slotIndex);
	if (weapon) {
		Server_EquipWeapon(weapon->weaponName);
		//if (!HasAuthority()) {
		//	Server_EquipWeapon(weapon->weaponName);
		//}
		//else {
		//	Multi_EquipWeapon(weapon->weaponName);
		//}
	}
}

void AFishingBattleCharacter::OnRep_EquipWeapon()
{
	if (weaponActor) {
		weaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("wepon"));
	}
}

void AFishingBattleCharacter::EquipSlot1()
{
	UE_LOG(LogTemp, Warning, TEXT("weapon1"));
	//EquipSlotIndex(1);
	if (!HasAuthority())
	{
		Server_EquipSlotIndex(1);
	}
	else {
		Multi_EquipSlotIndex(1);
	}
}

void AFishingBattleCharacter::EquipSlot2()
{
	UE_LOG(LogTemp, Warning, TEXT("weapon2"));
	//EquipSlotIndex(2);
	if (!HasAuthority())
	{
		Server_EquipSlotIndex(2);
	}
	else {
		Multi_EquipSlotIndex(2);
	}
}

void AFishingBattleCharacter::EquipSlot3()
{
	UE_LOG(LogTemp, Warning, TEXT("weapon3"));
	//EquipSlotIndex(3);
	if (!HasAuthority())
	{
		Server_EquipSlotIndex(3);
	}
	else {
		Multi_EquipSlotIndex(3);
	}
}

void AFishingBattleCharacter::EquipFishlot()
{
	UE_LOG(LogTemp, Warning, TEXT("fishing"));
	//EquipSlotIndex(0);
	if (!HasAuthority())
	{
		Server_EquipSlotIndex(0);
		//Multi_EquipSlotIndex(0);
	}
	else {
		Multi_EquipSlotIndex(0);
		//Server_EquipSlotIndex(0);
	}
}

void AFishingBattleCharacter::EquipWeapon(FName weaponID)
{
	if (weaponActor) {
		weaponActor->Destroy();
		weaponActor = nullptr;
	}


	//AGameMode_T* gm = Cast<AGameMode_T>(UGameplayStatics::GetGameMode(this));
	//if (!gm)return;

	//TSubclassOf<AActor> weaponClass = gm->GetWeaponClass(weaponID);
	//if (weaponClass) {
	//	FActorSpawnParameters spawnParams;
	//	spawnParams.Owner = this;
	//	AActor* newWeapon = GetWorld()->SpawnActor<AActor>(weaponClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);


	//	if (newWeapon) {


	//		UChildActorComponent* weaponChildComponent = nullptr;
	//		TArray<UChildActorComponent*> ChildActorComponents;
	//		GetComponents<UChildActorComponent>(ChildActorComponents);
	//		for (UChildActorComponent* comp : ChildActorComponents)
	//		{
	//			if (comp->GetName() == TEXT("wepon"))
	//			{
	//				weaponChildComponent = comp;
	//				break;
	//			}
	//		}
	//		if (weaponChildComponent)
	//		{
	//			weaponActor = newWeapon;
	//			weaponActor->SetOwner(this);
	//			TSubclassOf<AActor> WeaponClass = newWeapon->GetClass();
	//			weaponChildComponent->SetChildActorClass(weaponClass); // TSubclassOf<AActor>
	//		}

	//		//weaponActor = newWeapon;
	//		//weaponActor->SetOwner(this);
	//		//weaponActor->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("wepon"));
	//	}
	//}

	//-----------------------------------------------------------------------


	AGameMode_T* gm = Cast<AGameMode_T>(UGameplayStatics::GetGameMode(this));
	if (!gm)return;

	TSubclassOf<AActor> weaponClass = gm->GetWeaponClass(weaponID);
	UE_LOG(LogTemp, Warning, TEXT("weaponClass is: %s"), *weaponClass->GetName());
	if (weaponClass) {


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
		if (weaponChildComponent)
		{
			weaponChildComponent->SetChildActorClass(weaponClass); // TSubclassOf<AActor>

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


		////武器のタイプを取得
		//ACPPBaseWeapon* Base = Cast<ACPPBaseWeapon>(weaponActor);
		//if (Base)
		//{
		//	UE_LOG(LogTemp, Error, TEXT("GetWeaponTypeInCharacter!!!!!!"));
		//	this->WeaponType = Base->WeaponType;
		//	//OnRep_AnimInstance();

		//}

		//UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
        //if (!animInstance)return;
        //UMyAnimInstance* mAnim = Cast<UMyAnimInstance>(animInstance);
        //if (!mAnim)return;
        //mAnim->GetWeaponType(this->WeaponType);

		//weaponActor = newWeapon;
		//weaponActor->SetOwner(this);
		//weaponActor->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("wepon"));

	}
}

void AFishingBattleCharacter::ChangeMappingContext(UInputMappingContext* context_)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(nowMappingContext);
			Subsystem->AddMappingContext(context_, 0);
			nowMappingContext = context_;
		}
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

void AFishingBattleCharacter::DelayedCheckWeaponType()
{
	//武器のタイプを取得
	ACPPBaseWeapon* Base = Cast<ACPPBaseWeapon>(weaponActor);
	if (Base)
	{
		UE_LOG(LogTemp, Error, TEXT("GetWeaponTypeInCharacter!!!!!!"));
		this->WeaponType = Base->WeaponType;
		OnRep_AnimInstance();

	}
}

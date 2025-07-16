// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FishingBattleGameMode.h"
#include "Tokumaru/GameMode_T.h"
#include "GameFramework/Character.h"
#include "Tokumaru/MyAnimInstance.h"
#include "Logging/LogMacros.h"
#include "tokumaru/InventoryWeapon.h"
#include "Net/UnrealNetwork.h"
#include "TakimotoBranch/CPPBaseWeapon.h"
#include "TakimotoBranch/CPPWeaponType.h"
#include "FishingBattleCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UMyAnimInstance;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class FISHINGBATTLE_API AFishingBattleCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* FishingMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DeadMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FishingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchWeapon1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchWeapon2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchWeapon3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchFishlot;


	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* HeavyAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* RollMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* DeadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* FishingMontage;



public:
	AFishingBattleCharacter();

	/// <summary>
	/// すごい文字化けしてる
	/// </summary>
	/// <param name="DamageAmount"></param>
	/// <param name="DamageEvent"></param>
	/// <param name="EventInstigator"></param>
	/// <param name="DamageCauser"></param>
	/// <returns></returns>
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	float Health = 100;


	/// <summary>
	/// 釣り場侵入
	/// </summary>
	/// <param name="spot"></param>
	UFUNCTION(BlueprintCallable)
	void EnterSpot(AActor* spot);


	/// <summary>
	/// 釣り場退出
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ExitSpot();

	//釣りしたとき受け取るものと釣りができるかの判定用。
	int fish = 0;
	bool canFishing = false;
	AActor* fishingSpot = nullptr;








protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/// <summary>
	/// 攻撃モーション再生
	/// </summary>
	void Attack1();
	bool IsPlayAttack1 = false;

	/// <summary>
	/// 攻撃モーション終了
	/// </summary>
	void OnAttackEnded(UAnimMontage* Montage, bool in);

	/// <summary>
	/// 回避モーション再生
	/// </summary>
	void Roll();
	bool IsRoll = false;

	/// <summary>
	/// 回避モーション終了
	/// </summary>
	/// <param name="Montage"></param>
	/// <param name="in"></param>
	void OnRollEnded(UAnimMontage* Montage, bool in);

	/// <summary>
	/// ジャンプをさせないようにするために上書き
	/// </summary>
	virtual void Jump() override;

	/// <summary>
	/// 死亡モーション
	/// </summary>
	void Die();
	bool IsDead = false;

	/// <summary>
	/// 死亡モーション終了
	/// </summary>
	void OnDeadEnded(UAnimMontage* Montage, bool in);

	/// <summary>
	/// 釣りモーション再生
	/// </summary>
	void Fishing();
	bool IsFishing = false;

	/// <summary>
	/// 釣りモーション終了
	/// </summary>
	void OnFishingEnded(UAnimMontage* Montage, bool in);

	/// <summary>
	/// 死亡モーション終了後
	/// </summary>
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Die();  //　クライアント用

	/// <summary>
	/// 死亡した瞬間
	/// </summary>
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Dead();  //　クライアント用

	/// <summary>
	/// ゲームモード(サーバー)へのリスポーン要求
	/// </summary>
	void HandleDeath(); //　サーバー用


	/// <summary>
	/// リスポーン要求↑in
	/// </summary>
	void RequestRespawn(); //　リスポーンを要求

	FTimerHandle RespawnTimerHandle;

	/// <summary>
/// 死亡モーション終了後
/// </summary>
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Die();  // サーバー用

	/// <summary>
	/// 死亡した瞬間
	/// </summary>
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Dead();  // サーバー用

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Attack();  // クライアント用

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Roll();  // クライアント用

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fishing();  // クライアント用


	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Attack();  // サーバー用

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Roll();  //サーバー用

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Fishing();  // サーバー用










protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void BeginPlay() override;

	//インベントリ関連
public:
	//武器のアクター登録
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_EquipWeapon)
	AActor* weaponActor;

	//ゲームモードに移した
	//UPROPERTY(EditDefaultsOnly)
	//TMap<FName, TSubclassOf<AActor>> weaponMap;


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipWeapon(FName weaponID);//クライアント用

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_EquipWeapon(FName weaponID);  // サーバー用

	void EquipSlotIndex(int slotIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipSlotIndex(int slotIndex);//クライアント用

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_EquipSlotIndex(int slotIndex);  // サーバー用

	UFUNCTION()
	void OnRep_EquipWeapon();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddWeaponInPlayer(FName WeaponID); //クライアント用

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_AddWeaponInPlayer(FName WeaponID);  // サーバー用

	/// <summary>
	/// ゲームに入ったかどうかの判定をするもの
	/// </summary>
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TrueInGamePlay();//クライアント用


protected:

	/// <summary>
	/// スロット１の武器を装備
	/// </summary>
	void EquipSlot1();

	/// <summary>
	/// スロット２の武器を装備
	/// </summary>
	void EquipSlot2();

	/// <summary>
	/// スロット３の武器を装備
	/// </summary>
	void EquipSlot3();

	/// <summary>
	/// 釣り竿装備
	/// </summary>
	void EquipFishlot();

	void EquipWeapon(FName weaponID);

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthUpdate, float, maxHP, float, updateHP);



	UPROPERTY(BlueprintAssignable, Category = "Health")
	FHealthUpdate healthUpdate;


	UFUNCTION()
	void GetPlayerHealth(float maxHP, float updateHP);

	UFUNCTION(BlueprintCallable)
	void BroadcastHP(float maxHP, float updateHP);



protected:
	/// <summary>
	/// マッピングコンテクストの切り替えで操作を制限する。
	/// </summary>
	/// <param name="context_"></param>
	void ChangeMappingContext(UInputMappingContext* context_);
	//void RemoveMappingContext(UInputMappingContext* context_);
	UInputMappingContext* nowMappingContext = nullptr;

public:

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_AnimInstance, BlueprintReadWrite, Category = "Weapon")
	ECPPWeaponType WeaponType = ECPPWeaponType::None;

	UFUNCTION()
	void OnRep_AnimInstance();


	UFUNCTION()
	void DelayedCheckWeaponType();

public:
	UPROPERTY(BlueprintReadWrite,Category = "Fishing")
	bool canFish = false;
};


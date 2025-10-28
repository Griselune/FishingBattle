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
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "NiagaraComponent.h"
#include "TakimotoBranch/CPPBaseWeapon.h"
#include "TakimotoBranch/CPPWeaponType.h"
#include "FishingBattleCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UMyAnimInstance;
struct FInputActionValue;
class UWidgetComponent; //プリンス 追加 2025/10/21　ネームタグに使う

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class FISHINGBATTLE_API AFishingBattleCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	AFishingBattleCharacter();

	/// <summary>
	/// ダメージをもらう
	/// </summary>
	/// <param name="DamageAmount"></param>
	/// <param name="DamageEvent"></param>
	/// <param name="EventInstigator"></param>
	/// <param name="DamageCauser"></param>
	/// <returns></returns>
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void BeginPlay() override;

#pragma region インプット

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
	TObjectPtr<UNiagaraComponent> effect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
	TObjectPtr<UNiagaraComponent> damageEffect;


	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* FishingMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DeadMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* HasFishrotMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* HasweaponMappingContext;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DestructionWeaponInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GaugeStop;

protected:
	/// <summary>
	/// マッピングコンテクストの切り替えで操作を制限する。
	/// </summary>
	/// <param name="context_"></param>
	void ChangeMappingContext(UInputMappingContext* context_);
	//void RemoveMappingContext(UInputMappingContext* context_);
	UInputMappingContext* nowMappingContext = nullptr;
#pragma endregion

#pragma region アニメーション
public:
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

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* UpLot;
#pragma endregion


#pragma region キャラクターステータス
private:

	/// <summary>
	/// 無敵時間作成
	/// </summary>
	UPROPERTY(Replicated,ReplicatedUsing = OnRep_UnDead)
	bool UnDead = true;
	UFUNCTION()
	void OnRep_UnDead();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP", meta = (AllowPrivateAccess = "true"), Replicated, ReplicatedUsing = OnRep_UpdatedHealth)
	float Health = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HP", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	//10.26　滝本　start
	UPROPERTY(EditAnywhere, Category = "HP")
	USoundBase* HealSound;
	//10.26　滝本　end

	/// <summary>
	/// プレイヤーステートの死に回数計測変数に加算。
	/// </summary>
	void AddToDeadCounter();
#pragma endregion

#pragma region 外部から呼び出し
public:

	/// <summary>
	/// 海に入った
	/// </summary>
	void EnterSea(AActor* Actor);

	/// <summary>
	/// 最大HPをゲット
	/// </summary>
	/// <returns>最大HP</returns>
	float GetMaxHealth() const;

	// 2025.10.23 ウー start
	/// <summary>
	/// ヒールする
	/// </summary>
	/// <param name="healAmount">回復量</param>
	UFUNCTION(Server, Reliable)
	void Heal(float healAmount, float Point);
	// 2025.10.23 ウー end

	/// <summary>
	/// ヒールエフェクト表示
	/// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void Multi_Heal();  // サーバー用

	/// <summary>
    /// ヒールエフェクト表示
    /// </summary>
	UFUNCTION(Server, Reliable)
	void Server_Heal();  // クライアント用

	/// <summary>
	/// ダメージエフェクト表示用
	/// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void Multi_DamageEffect();  // サーバー用

	/// <summary>
	/// ダメージエフェクト表示用
	/// </summary>
	UFUNCTION(Server, Reliable)
	void Server_DamageEffect();  // クライアント用

	///// <summary>
 //   /// 無敵エフェクト表示用
 //   /// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void Multi_UndeadEffect();  // サーバー用

	///// <summary>
	///// 無敵エフェクト表示用
	///// </summary>
	UFUNCTION(Server, Reliable)
	void Server_UndeadEffect();  // クライアント用

	///// <summary>
 //   /// エフェクト停止用
 //   /// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void Multi_StopBodyEffect();  // サーバー用

	///// <summary>
	///// エフェクト停止用
	///// </summary>
	UFUNCTION(Server, Reliable)
	void Server_StopBodyEffect();  // クライアント用

	UPROPERTY(EditAnywhere, Category = "effect")
	UNiagaraSystem* damageAsset;

	UPROPERTY(EditAnywhere, Category = "effect")
	UNiagaraSystem* unDeadAsset;


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

	//釣りが可能かどうか
	UPROPERTY(EditAnywhere)
	bool canFishing = false;

private:
	/// <summary>
	/// 海
	/// </summary>
	AActor* Sea;

	/// <summary>
	/// 釣り場
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_FishingSpot)
	AActor* fishingSpot = nullptr;
	UFUNCTION()
	void OnRep_FishingSpot();

	/// <summary>
	/// 今装備している武器のインベントリ番号
	/// </summary>
	int nowInventoryIndex = 0;
#pragma endregion

#pragma region アニメーションモンタージュ再生から終了までの処理
public:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_IsFishing)
	bool IsFishing = false;

	UFUNCTION()
	void OnRep_IsFishing();
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
	bool CanRoll = true;

	/// <summary>
	/// 回避にクールタイムを追加する。boolの切り替え。
	/// tickを使って時間でクールタイムをつけてもよいが、サーバーとの兼ね合いもあるので、
	/// いったんboolの操作で解決する。
	/// </summary>
	void ReloadCanRoll();

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

	/// <summary>
	/// 釣りモーション終了
	/// </summary>
	void OnFishingEnded(bool Result);
	//10月15日　滝本海大　開始
	//釣り場から貰ってきた魚を保存する変数
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_WeaponActorSubclass)
	TSubclassOf<AActor> weaponActorSubclass;

	UFUNCTION()
	void OnRep_WeaponActorSubclass();
		
	//IA_GaugeStopにバインドする関数
	void OnGaugeStop();

	//釣りゲージを出す関数(アニメーションに通知するやつ)
	UFUNCTION()
	void ShowFishingGauge(UAnimMontage* Montage, bool in);
	//10月15日　滝本海大　終了
#pragma endregion

#pragma region RPC関数
protected:
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
	/// 死亡モーション終了後
	/// </summary>
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Die();  // サーバー用

	/// <summary>
	/// 死亡した瞬間
	/// </summary>
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Dead();  // サーバー用

	/// <summary>
	/// 攻撃開始
	/// </summary>
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Attack();  // クライアント用

	/// <summary>
	/// 回避開始
	/// </summary>
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Roll();  // クライアント用

	/// <summary>
	/// 釣り開始
	/// </summary>
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fishing();  // クライアント用

	/// <summary>
	/// 攻撃開始
	/// </summary>
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Attack();  // サーバー用

	/// <summary>
	/// 回避開始
	/// </summary>
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Roll();  //サーバー用

	/// <summary>
	/// 釣り開始
	/// </summary>
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Fishing();  // サーバー用

	/// <summary>
	/// 武器装備
	/// </summary>
	/// <param name="weaponID"></param>
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipWeapon(TSubclassOf<AActor> weaponID);//クライアント用

	/// <summary>
	/// 武器装備
	/// </summary>
	/// <param name="weaponID"></param>
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_EquipWeapon(TSubclassOf<AActor> weaponID);  // サーバー用

	/// <summary>
	/// 引数を使ってインベントリに武器があるか調べる
	/// </summary>
	/// <param name="slotIndex"></param>
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EquipSlotIndex(int slotIndex);//クライアント用

	/// <summary>
    /// 引数を使ってインベントリに武器があるか調べる
    /// </summary>
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_EquipSlotIndex(int slotIndex);  // サーバー用

	/// <summary>
	/// プレイヤーステートの中のインベントリ配列に武器を追加する。
	/// </summary>
	/// <param name="WeaponID"></param>
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_AddWeaponInPlayer(TSubclassOf<AActor> WeaponID);  // サーバー用

	/// <summary>
	/// ゲームに入ったかどうかの判定をするもの
	/// </summary>
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TrueInGamePlay();//クライアント用

	/// <summary>
	/// ゲーム開始時にインベントリ配列に釣り竿追加する。
	/// </summary>
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_BeginAddFishrot();//クライアント用

	/// <summary>
	/// ゲーム開始時にインベントリ配列に釣り竿追加する。
	/// </summary>
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_BeginAddFishrot();//クライアント用


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DestructionWeapon(int index);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_DestructionWeapon(int index);

	//10月15日　滝本海大　開始
	//釣りゲージの結果から魚を手に入れるかどうかを判定する
	UFUNCTION(Server, Reliable)
	void Server_GetFishByGauge(bool Result);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_GetFishByGauge(bool Result);
	//10月15日　滝本海大　終了

	UFUNCTION(Server, Reliable)
	void Server_EnterSpot(AActor* spot);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_EnterSpot(AActor* spot);

	UFUNCTION(Server, Reliable)
	void Server_ExitSpot();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_ExitSpot();
#pragma endregion


#pragma region オーバーライド
protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// プレイヤーがコントローラーに所持されたとき呼ばれる.
	/// プレイヤーステートも取得できる
	/// </summary>
	/// <param name="NewController"></param>
	virtual void PossessedBy(AController* NewController) override;

	// 2025.07.30 ウー start
	/// <summary>
	/// プレイヤーがPlayerStateを与えられた時
	/// </summary>
	virtual void OnRep_PlayerState() override;
	// 2025.07.30 ウー end
#pragma endregion

#pragma region 武器装備
public:
	//武器のアクター登録用
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_EquipWeapon)
	AActor* weaponActor;

	/// <summary>
	/// 最初に装備できる釣り竿を指定
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> FishRod;

	/// <summary>
	/// 所持中の武器が切り替わったときに呼ばれる現状logのみ
	/// </summary>
	UFUNCTION()
	void OnRep_EquipWeapon();

	/// <summary>
	/// 武器のタイプを取得。
	/// 装備した直後だと武器の初期化が終わっていない可能性があるので,遅延させている。
	/// </summary>
	UFUNCTION()
	void DelayedCheckWeaponType();

	/// <summary>
	/// 現在装備している武器のタイプ
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_AnimInstance, BlueprintReadWrite, Category = "Weapon")
	ECPPWeaponType WeaponType = ECPPWeaponType::None;

	/// <summary>
	/// 武器のタイプが切り替わったときに呼ばれる
	/// </summary>
	UFUNCTION()
	void OnRep_AnimInstance();
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

	/// <summary>
	/// 武器を装備する
	/// </summary>
	/// <param name="weaponID"></param>
	void EquipWeapon(TSubclassOf<AActor> weaponID);

	/// <summary>
	/// 引数の番号でインベントリを参照して武器があるか確認
	/// </summary>
	/// <param name="slotIndex"></param>
	void EquipSlotIndex(int slotIndex);

	/// <summary>
	/// 武器の破棄
	/// </summary>
	void DestructionWeapon();
#pragma endregion

#pragma region 体力UIに使用
public:

	/// <summary>
	/// リスポーンの後、UIに何か更新が必要な時、ここで更新する
	/// </summary>
	virtual void OnRep_Controller() override;

	/// <summary>
	/// HPの更新
	/// </summary>
	/// <param name="MaxHP">最大HP</param>
	/// <param name="NewHP">新しいHP</param>
	void UpdateHP(float MaxHP, float NewHP);

	UFUNCTION()
	void OnRep_UpdatedHealth();

#pragma endregion

#pragma region リスポーン
protected:

	/// <summary>
	/// ゲームモード(サーバー)へのリスポーン要求
	/// </summary>
	void HandleDeath(); //　サーバー用


	/// <summary>
	/// リスポーン要求
	/// </summary>
	void RequestRespawn(); //　リスポーンを要求
#pragma endregion

// 
#pragma region データ設定
	/// <summary>
	/// 名前を設定
	/// </summary>
	void SetNameFromInstance();
#pragma endregion

#pragma region 名前表示（ネームタグ）
	//プリンス START 2025/10/21
public:
	/// <summary>
	/// 名前ウィジェットを更新
	/// </summary>
	void UpdateNameWidget();

	/// <summary>
	/// ポイントウィジェットを更新
	/// </summary>
//	UFUNCTION(NetMulticast, Reliable)
	void UpdateDisplayPointsWidget();
private:
	/// <summary>
	/// 名前が更新されたとき
	/// </summary>
	UFUNCTION()
	void OnRep_UpdatedName();

	/// <summary>
	/// ポイントが更新されたとき
	/// </summary>
	UFUNCTION()
	void OnRep_UpdatedPoints();

	/// <summary>
	/// プレイヤ名
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_UpdatedName)
	FString Name;

	/// <summary>
	/// プレイヤのポイント（スコア）
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_UpdatedPoints)
	int32 DPoints;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//UWidgetComponent* NameTagWidgetComp;

	protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Child Actor")
	UChildActorComponent* ChildActorComp;


public:
	virtual void PostNetInit() override;

	UFUNCTION(Server, Reliable)
	void ServerSetPlayerName(const FString& NewName);

	UFUNCTION(Server, Reliable)
	void ServerSetPlayerDPoints(const int32& NewPoints);
	//プリンス END 2025/10/21

public:
	/// <summary>
	/// 名前を設定
	/// </summary>
	/// <param name="NewName"></param>

	void SetName(const FString& NewName);

	/// <summary>
	/// 名前をゲット
	/// </summary>
	/// <returns></returns>
	FString GetName() const;

	/// <summary>
	/// ポイントの設定
	/// </summary>
	/// <param name="NewPoints"></param>
	void SetDPoints(const int32& NewPoints);

	/// <summary>
	/// ポイントのゲット
	/// </summary>
	/// <returns></returns>
	int32 GetPoints() const;

#pragma endregion

// 2025.10.24 ウー start
#pragma region 王冠
public:

	/// <summary>
	/// 王冠を表示
	/// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void ShowCrown();

	/// <summary>
	/// 王冠を非表示
	/// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void HideCrown();

private:

	/// <summary>
	/// 王冠のセットアップ
	/// </summary>
	void SetupCrown();

	/// <summary>
	/// 王冠のメッシュ
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Crown;
#pragma endregion
// 2025.10.24 ウー end

// 2025.10.22 ウー start
#pragma region ポイント
public:

	/// <summary>
	/// ポイント加算
	/// </summary>
	/// <param name="Point">ポイント</param>
	void AddPoint(float Point);

	/// <summary>
	/// ポイントをゲット
	/// </summary>
	/// <returns>ポイント</returns>
	float GetPoint();

private:

	/// <summary>
	/// プレイヤーを倒したのポイント処理
	/// </summary>
	/// <param name="DamageCauser">ダメージ元</param>
	void AwardPointsForDefeat(AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, Category = "Point", meta = (AllowPrivateAccess = "true"))
	float AttackPoint;
#pragma endregion
// 2025.10.22 ウー end
};


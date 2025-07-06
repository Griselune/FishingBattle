// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FishingBattleGameMode.h"
#include "GameFramework/Character.h"
#include "Tokumaru/MyAnimInstance.h"
#include "Logging/LogMacros.h"
#include "FishingBattleCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UMyAnimInstance;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AFishingBattleCharacter : public ACharacter
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

	UPROPERTY(EditDefaultsOnly,Category = "Anim")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* RollMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* DeadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* FishingMontage;

public:
	AFishingBattleCharacter();


	/// <summary>
    /// ï¿½_ï¿½ï¿½ï¿½[ï¿½Wï¿½Ìó‚¯ï¿½ï¿½
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
	/// ï¿½Ş‚ï¿½ï¿½Nï¿½ï¿½ï¿½ï¿½
	/// </summary>
	/// <param name="spot"></param>
	UFUNCTION(BlueprintCallable)
	void EnterSpot(AActor* spot);


	/// <summary>
	/// ï¿½Ş‚ï¿½ê—£ï¿½Eï¿½ï¿½
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ExitSpot();

	//ï¿½ï¿½ï¿½ï¿½ç‚¤
	int fish = 0;
	bool canFishing = false;
	AActor* fishingSpot = NULL;

	


	



protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/// <summary>
	/// ï¿½Uï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Jï¿½n
	/// </summary>
	void Attack1();
	bool IsPlayAttack1 = false;

	/// <summary>
	/// ï¿½Uï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½
	/// </summary>
	void OnAttackEnded(UAnimMontage* Montage, bool in);

	/// <summary>
	/// ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Jï¿½n
	/// </summary>
	void Roll();
	bool IsRoll = false;

	/// <summary>
	/// ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½
	/// </summary>
	/// <param name="Montage"></param>
	/// <param name="in"></param>
	void OnRollEnded(UAnimMontage* Montage, bool in);

	/// <summary>
	/// ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ß‚Éã‘ï¿½ï¿½
	/// </summary>
	virtual void Jump() override;

	/// <summary>
	/// ï¿½ï¿½ï¿½Sï¿½p
	/// </summary>
	void Die();
	bool IsDead = false;

	/// <summary>
	/// ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½Nï¿½^ï¿½[ï¿½ï¿½ï¿½ï¿½
	/// </summary>
	void OnDeadEnded(UAnimMontage* Montage, bool in);

	/// <summary>
	/// ï¿½Ş‚ï¿½Jï¿½nï¿½B
	/// </summary>
	void Fishing();
	bool IsFishing = false;

	/// <summary>
	/// ï¿½Ş‚ï¿½Iï¿½ï¿½
	/// </summary>
	void OnFishingEnded(UAnimMontage* Montage, bool in);

	UFUNCTION(Server, Reliable)
	void Server_Die();  // ï¿½Nï¿½ï¿½ï¿½Cï¿½Aï¿½ï¿½ï¿½gï¿½p

	void HandleDeath(); // ï¿½Tï¿½[ï¿½oï¿½[ï¿½ï¿½ï¿½ï¿½p

	void RequestRespawn(); // ï¿½^ï¿½Cï¿½}ï¿½[ï¿½ÅŒÄ‚ï¿½

	FTimerHandle RespawnTimerHandle;


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Attack();  // ƒNƒ‰ƒCƒAƒ“ƒg—p

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Roll();  // ƒNƒ‰ƒCƒAƒ“ƒg—p

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fishing();  // ƒNƒ‰ƒCƒAƒ“ƒg—p


	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void Multi_Attack();  // ƒT[ƒo[—p

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Roll();  //ƒT[ƒo[—p

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Fishing();  // ƒT[ƒo[—p






			

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
};


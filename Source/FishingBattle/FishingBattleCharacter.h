// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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


	UPROPERTY(EditDefaultsOnly,Category = "Anim")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* RollMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* DeadMontage;

public:
	AFishingBattleCharacter();


	/// <summary>
    /// �_���[�W�̎󂯎��
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

	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/// <summary>
	/// �U�����[�V�����J�n
	/// </summary>
	void Attack1();
	bool IsPlayAttack1 = false;

	/// <summary>
	/// �U�����[�V�������I��
	/// </summary>
	void OnAttackEnded(UAnimMontage* Montage, bool in);

	/// <summary>
	/// ������[�V�����J�n
	/// </summary>
	void Roll();
	bool IsRoll = false;

	/// <summary>
	/// �U�����[�V�������I��
	/// </summary>
	/// <param name="Montage"></param>
	/// <param name="in"></param>
	void OnRollEnded(UAnimMontage* Montage, bool in);

	/// <summary>
	/// �W�����v�������Ȃ����߂ɏ㏑��
	/// </summary>
	virtual void Jump() override;

	/// <summary>
	/// ���S�p
	/// </summary>
	void Die();
	bool IsDead = false;

	/// <summary>
	/// �L�����N�^�[����
	/// </summary>
	void OnDeadEnded(UAnimMontage* Montage, bool in);







			

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
};


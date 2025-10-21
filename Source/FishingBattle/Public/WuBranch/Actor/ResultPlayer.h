// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WuBranch/PlayerState/ResultState.h"
#include "ResultPlayer.generated.h"

class UCapsuleComponent;
class UInputMappingContext;
class UInputAction;
class UWidgetComponent;

UCLASS()
class FISHINGBATTLE_API AResultPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AResultPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// コリジョーンの幅さをゲット
	/// </summary>
	/// <returns></returns>
	float GetCollisionWidth() const;

private:

	/// <summary>
	/// 体のコリジョーン
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* BodyCollision;

	/// <summary>
	/// メッシュ
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

#pragma region UUID
public:

	/// <summary>
	/// UUIDを設定
	/// </summary>
	/// <param name="ID">ID</param>
	void SetUUID(int ID);

	/// <summary>
	/// UUIDをゲット
	/// </summary>
	/// <returns>ID</returns>
	UFUNCTION(BlueprintCallable)
	int GetUUID() const;

private:

	/// <summary>
	/// UUIDが更新されたとき
	/// </summary>
	UFUNCTION()
	void OnRep_UpdatedUUID();

	/// <summary>
	/// ゲーム中のUUID
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_UpdatedUUID)
	int UUID;
#pragma endregion

#pragma region 名前
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

private:

	/// <summary>
	/// 名前が更新されたとき
	/// </summary>
	UFUNCTION()
	void OnRep_UpdatedName();

	/// <summary>
	/// 名前ウィジェットを更新
	/// </summary>
	void UpdateNameWidget();

	/// <summary>
	/// プレイヤ名
	/// </summary>
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_UpdatedName)
	FString Name;

	/// <summary>
	/// 名前のウィジェットコンポーネント
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* NameWidgetComp;
#pragma endregion

#pragma region 状態

public:
	/// <summary>
	/// プレイヤーの結果を設定する
	/// </summary>
	/// <param name="NewState">結果</param>
	UFUNCTION(NetMulticast, Reliable)
	void SetResultState(EResultState NewState);

	/// <summary>
	/// 結果がわかったですか
	/// </summary>
	/// <returns>true: はい, false: いいえ</returns>
	bool IsKnowResult() const;

	/// <summary>
	/// 勝者ですか
	/// </summary>
	/// <returns>true: はい, false: いいえ</returns>
	bool IsWinner() const;

private:

	/// <summary>
	/// 今の状態
	/// </summary>
	EResultState CurrentState;

#pragma endregion

#pragma region 入力関連
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// デフォルト入力を有効にする
	/// </summary>
	void EnableDefaultInput();

private:

	/// <summary>
	/// 拍手
	/// </summary>
	void Clap();

	/// <summary>
	/// 拍手(サーバー)
	/// </summary>
	UFUNCTION(Server, Reliable)
	void Server_Clap();

	/// <summary>
	/// 大拍手
	/// </summary>
	void BigClap();

	/// <summary>
	/// 大拍手(サーバー)
	/// </summary>
	UFUNCTION(Server, Reliable)
	void Server_BigClap();

	/// <summary>
	/// デフォルト入力のマップイング
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/// <summary>
	/// 拍手アクション
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ClapAction;

	/// <summary>
	/// 大拍手アクション
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BigClapAction;

	/// <summary>
	/// 歓声アクション
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CheerAction;

	/// <summary>
	/// 勝利アクション
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VictoryAction;

#pragma endregion


};

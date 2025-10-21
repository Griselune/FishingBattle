// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResultSpotLight.generated.h"

class USpotLightComponent;
class USphereComponent;
class AResultPlayer;

UCLASS()
class FISHINGBATTLE_API AResultSpotLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResultSpotLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// 移動開始
	/// </summary>
	UFUNCTION(Server, Reliable)
	void StartMove();

	/// <summary>
	/// 勝者を設定
	/// </summary>
	UFUNCTION(Server, Reliable)
	void SetWinner(int ID);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPointWinnerDelegate);

	/// <summary>
	/// 勝者を指したかの登録
	/// </summary>
	UPROPERTY(BlueprintAssignable)
	FPointWinnerDelegate OnPointed;

private:

	/// <summary>
	/// 初めのライトの照明対象を設定
	/// </summary>
	void SetFirstLightTarget();

	/// <summary>
	/// ランダムで新しい目標を設定
	/// </summary>
	/// <returns></returns>
	AActor* GetNewRandomTarget();

	/// <summary>
	/// 経過した演出時間の計算
	/// </summary>
	/// <param name="DeltaTime">時間差</param>
	void CountEffectTime(float DeltaTime);

	/// <summary>
	/// 新しい目標に移動
	/// </summary>
	/// <param name="DeltaTime">時間差</param>
	void MoveToNewTarget(float DeltaTime);

	/// <summary>
	/// ライトを指定された場所に照らす
	/// </summary>
	/// <param name="Location">位置</param>
	void PointLightToLocation(FVector TargetLocation);

	/// <summary>
	/// 強制的に最後の目標を設定
	/// </summary>
	/// <param name="Target">目標</param>
	void ForceSetLastTarget(AActor* Target);

	/// <summary>
	/// 勝者のキャラクターを見つける
	/// </summary>
	AActor* FindWinnerCharacter();

	/// <summary>
	/// 勝者を指したかのを通知
	/// </summary>
	void NotifyPointedWinnerEvent();

	/// <summary>
	/// ライト
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	USpotLightComponent* SpotLight;

	/// <summary>
	/// 全プレイヤーのキャラクター
	/// </summary>
	TArray<AActor*> Characters;

	/// <summary>
	/// 現在の照明対象
	/// </summary>
	AActor* CurrentTarget;

	/// <summary>
	/// 次の照明対象
	/// </summary>
	AActor* NextTarget;

	/// <summary>
	/// 勝者を見つける演出の時間
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float EffectDuration;

	/// <summary>
	/// 演出時間のカウント
	/// </summary>
	float EffectTimeCnt;

	/// <summary>
	/// 演出終了ですか
	/// </summary>
	bool bIsEffectFinish;

	/// <summary>
	/// 移動開始ですか
	/// </summary>
	//UPROPERTY(Replicated)
	bool CanStart;

	/// <summary>
	/// 勝者のID
	/// </summary>
	//UPROPERTY(Replicated)
	int WinnerID;

	/// <summary>
	/// キャラクターの間に移動する時間
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MovingDuration;

	/// <summary>
	/// 移動時間のカウント
	/// </summary>
	float MovingTimeCnt;

	/// <summary>
	/// 今の照明対象の位置
	/// </summary>
	FVector CurrentTargetLocation;
};

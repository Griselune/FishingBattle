// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ResultPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API AResultPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AResultPlayerController();

protected:
	
	virtual void BeginPlay() override;

public:

	/// <summary>
	/// HUDのウィジェットをゲット
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns>ウィジェット</returns>
	template<class T>
	T* GetHUDWidget()
	{
		return Cast<T>(HUDWidget);
	}

	/// <summary>
	/// 勝者の名前を表示
	/// </summary>
	UFUNCTION(Client, Reliable)
	void ShowWinnerName();

private:

	/// <summary>
	/// ベースUIを画面に追加
	/// </summary>
	UFUNCTION(Client, Reliable)
	void AddWidgetToView();

	/// <summary>
	/// 勝者の名前を設定
	/// </summary>
	UFUNCTION(Client, Reliable)
	void SetWinnerName();

	/// <summary>
	/// 勝者の名前をリクエスト
	/// </summary>
	UFUNCTION(Server, Reliable)
	void RequestWinnerName();

	/// <summary>
	/// 勝者の名前を受け取る
	/// </summary>
	/// <param name="InName">勝者の名前</param>
	UFUNCTION(Client, Reliable)
	void ReceiveWinnerName(const FString& WinnerName);


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HUDUIClass;

	/// <summary>
	/// HUD Widget
	/// </summary>
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUDWidget;
};

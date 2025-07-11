// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class FISHINGBATTLE_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ABaseHUD();

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

private:

	/// <summary>
	/// ベースUIを画面に追加
	/// </summary>
	void AddWidgetToView();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HUDUIClass;

	/// <summary>
	/// HUD Widget
	/// </summary>
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUDWidget;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "WuBranch/Actor/ResultPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <WuBranch/PlayerState/ResultPlayerState.h>
#include <Net/UnrealNetwork.h>
#include "WuBranch/Interface/NameUI.h"

// Sets default values
AResultPlayer::AResultPlayer()
	: CurrentState(EResultState::Wait)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	BodyCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	BodyCollision->SetCollisionProfileName(FName("Pawn"));
	RootComponent = BodyCollision;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	NameWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameWidget"));
	NameWidgetComp->SetupAttachment(RootComponent);

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AResultPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentState = EResultState::Wait;

	if(NameWidgetComp && NameWidgetComp->GetWidget())
	{
		UpdateNameWidget();
	}
}

void AResultPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AResultPlayer, UUID);
	DOREPLIFETIME(AResultPlayer, Name);
}

// Called every frame
//void AResultPlayer::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

float AResultPlayer::GetCollisionWidth() const
{
	return BodyCollision->GetScaledCapsuleRadius();
}

#pragma region UUID

void AResultPlayer::SetUUID(int ID)
{
	UUID = ID;
}

int AResultPlayer::GetUUID() const
{
	return UUID;
}

void AResultPlayer::OnRep_UpdatedUUID()
{

}

#pragma endregion

#pragma region 名前
void AResultPlayer::SetName(const FString& NewName)
{
	Name = NewName;
	UpdateNameWidget();
}

FString AResultPlayer::GetName() const
{
	return Name;
}

void AResultPlayer::OnRep_UpdatedName()
{
	UpdateNameWidget();
}

void AResultPlayer::UpdateNameWidget()
{
	// 名前ウィジェットを更新
	if (UUserWidget* Widget = NameWidgetComp->GetWidget())
	{
		if (Widget->Implements<UNameUI>())
		{
			INameUI::Execute_SetName(Widget, Name);
		}
	}
}
#pragma endregion

void AResultPlayer::SetResultState_Implementation(EResultState NewState)
{
	CurrentState = NewState;
}

bool AResultPlayer::IsKnowResult() const
{
	return CurrentState != EResultState::Wait;
}

bool AResultPlayer::IsWinner() const
{
	return CurrentState == EResultState::Winner;
}

#pragma region 入力関連
// Called to bind functionality to input
void AResultPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!DefaultMappingContext || !ClapAction || !BigClapAction || !CheerAction || !VictoryAction)
		return;

	EnableDefaultInput();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
	}
	else
	{
		//UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AResultPlayer::EnableDefaultInput()
{
	APlayerController* MyController = Cast<APlayerController>(GetController());

	if (MyController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyController->GetLocalPlayer()))
		{
			if(!Subsystem->HasMappingContext(DefaultMappingContext))
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AResultPlayer::Clap()
{
}

void AResultPlayer::Server_Clap_Implementation()
{
}

void AResultPlayer::BigClap()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Big Clap in local"));
	// サーバーでなければサーバーに送る
	if (!HasAuthority()) {
		Server_BigClap();
	}
	else
	{

	}
}

void AResultPlayer::Server_BigClap_Implementation()
{
	if (!HasAuthority())
		return;
	AResultPlayerState* MyPlayerState = GetPlayerState<AResultPlayerState>();
}

#pragma endregion
// Fill out your copyright notice in the Description page of Project Settings.


#include "PrinzBranch/ChildPlayerNameTag.h"
#include "Components/WidgetComponent.h"


UWidgetComponent* AChildPlayerNameTag::GetNameTagWidgetComp() const
{
    return NameTagWidgetComp;
}

// Sets default values
AChildPlayerNameTag::AChildPlayerNameTag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// Create and configure the widget component
	NameTagWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameTagWidgetComp"));
	SetRootComponent(NameTagWidgetComp);

}

// Called when the game starts or when spawned
void AChildPlayerNameTag::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChildPlayerNameTag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


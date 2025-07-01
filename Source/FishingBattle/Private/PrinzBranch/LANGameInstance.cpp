// Fill out your copyright notice in the Description page of Project Settings.

#include "PrinzBranch/LANGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"


void ULANGameInstance::CreateLANSession(const FString& SessionName, const FString& Password)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            // Bind callback
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ULANGameInstance::OnCreateSessionComplete);

            SessionSettings = MakeShareable(new FOnlineSessionSettings());
            SessionSettings->bIsLANMatch = true;
            SessionSettings->NumPublicConnections = 4;
            SessionSettings->bShouldAdvertise = true;
            SessionSettings->bUsesPresence = false;

            // Store password in session settings
            SessionSettings->Set(FName("SessionPassword"), Password, EOnlineDataAdvertisementType::ViaOnlineService);

            SessionInterface->CreateSession(0, FName(*SessionName), *SessionSettings);
        }
    }
}

void ULANGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
     //   UE_LOG(LogTemp, Log, TEXT("LAN session '%s' created successfully"), *SessionName);

        // Open the map as listen server
        UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonMap"), true, "listen");
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create session."));
    }
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PrinzBranch/IPUtilityLibrary.h"

#include "Sockets.h"
#include "SocketSubsystem.h"

FString UIPUtilityLibrary::GetLocalIPAddress()
{
    bool bCanBind = false;
    TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind);

    if (!Addr->IsValid())
    {
        return FString("IP Not Found");
    }

    return Addr->ToString(false); // false = exclude port
}
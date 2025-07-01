// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FishingBattle : ModuleRules
{
	public FishingBattle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Sockets", "Networking", "OnlineSubsystem",
    "OnlineSubsystemUtils" });
	}
}

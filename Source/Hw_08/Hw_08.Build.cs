// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Hw_08 : ModuleRules
{
	public Hw_08(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Eira : ModuleRules
{
	public Eira(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"EnhancedInput",
			"GameplayTags",
			"GameplayTasks",
			"GameplayAbilities",
			"Niagara",
			"UMG",
			"Slate",
			"SlateCore"
		});

		PublicIncludePaths.Add("Eira");
	}
}

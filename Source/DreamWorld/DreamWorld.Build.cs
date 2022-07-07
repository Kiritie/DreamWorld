// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

/// <summary>
/// 
/// </summary>
public class DreamWorld : ModuleRules
{
	public DreamWorld(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivatePCHHeaderFile = "DreamWorld.h";

		bLegacyPublicIncludePaths = false;

		ShadowVariableWarningLevel = WarningLevel.Error;

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"WHFramework"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"InputCore",
				"SlateCore",
				"UMG",
				"AIModule",
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks",
				"ProceduralMeshComponent",
				"AchievementSystem",
				"TargetSystem"
			}
		);
	}
}

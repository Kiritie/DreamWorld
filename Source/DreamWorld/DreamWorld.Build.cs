// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

/// <summary>
/// 
/// </summary>
public class DreamWorld : ModuleRules
{
	public DreamWorld(ReadOnlyTargetRules Target) : base(Target)
	{
		bLegacyPublicIncludePaths = false;

		ShadowVariableWarningLevel = WarningLevel.Error;

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"WHFramework",
				"WHFrameworkCore",
				"WHFrameworkSlate"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"InputCore",
				"SlateCore",
				"Slate",
				"UMG",
				"CommonUI",
				"AIModule",
				"CommonInput",
				"EnhancedInput",
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks",
				"ProceduralMeshComponent",
				"DialoguePlugin"
			}
		);
	}
}

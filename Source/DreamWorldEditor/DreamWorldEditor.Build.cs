// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DreamWorldEditor : ModuleRules
{
	public DreamWorldEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		bLegacyPublicIncludePaths = false;

		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"WHFramework",
				"WHFrameworkEditor",
			});
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine",
				"Kismet",
				"Slate",
				"SlateCore",
				"EditorStyle",
				"ApplicationCore",
				"PropertyEditor",
				"AssetTools",
				"ClassViewer",
				"UnrealEd",
				"BlueprintGraph",
				"LevelEditor",
				"GraphEditor"
			});
	}
}

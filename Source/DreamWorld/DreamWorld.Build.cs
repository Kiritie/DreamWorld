// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

/// <summary>
/// ��ģ�����C#��
/// </summary>
public class DreamWorld : ModuleRules
{
	public DreamWorld(ReadOnlyTargetRules Target) : base(Target)
	{
		// ����Ԥ����ͷ�ļ�
		PrivatePCHHeaderFile = "DreamWorld.h";

		bLegacyPublicIncludePaths = false;

		ShadowVariableWarningLevel = WarningLevel.Error;

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// ��ӹ�������ģ��
		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"WHFramework"
			}
		);

		// ���˽������ģ��
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
				"WHFramework",
				"AchievementSystem",
				"TargetSystem"
			}
		);
	}
}

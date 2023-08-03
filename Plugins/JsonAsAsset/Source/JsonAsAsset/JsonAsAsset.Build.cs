// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JsonAsAsset : ModuleRules
{
	public JsonAsAsset(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp20;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"Json",
			"JsonUtilities",
			"UMG",
			"RenderCore",
            "UMGEditor",
            "HTTP",
			"DeveloperSettings",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Projects",
			"InputCore",
			"UnrealEd",
			"ToolMenus",
			"CoreUObject",
            "BlueprintGraph",
            "Engine",
			"Kismet",
            "KismetCompiler",
            "Slate",
			"SlateCore",
			"AnimationDataController",
			"MaterialEditor",
			"Landscape",
			"AssetTools",
			"EditorStyle",
			"Settings",
			"PhysicsCore",
            "MessageLog",
			"ToolWidgets",
			"PluginUtils",
			"RHI",
			"Detex",
			"NVTT"
		});
	}
}
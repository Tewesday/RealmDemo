// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RealmDemo : ModuleRules
{
	public RealmDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(
			new string[] { 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"EnhancedInput", 
				"StructUtils", 
				"DeveloperSettings", 
				"UMG", 
				"MassEntity", 
				"MassSpawner", 
				"MassCommon", 
				"MassEntityDebugger",
                "MassMovement",
                "MassGameplayDebug",
                "MassSignals",
                "MassCrowd",
                "MassActors",
                "MassRepresentation",
                "MassReplication",
                "MassNavigation",
				"NetCore",
                "AIModule",
                "GeometryCore",
                "Chaos",
                "PhysicsCore",
                "ChaosCore",
                "ChaosSolverEngine"
            });

		PrivateDependencyModuleNames.AddRange(
			new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "EnhancedInput",
                "StructUtils",
                "DeveloperSettings",
                "UMG",
                "MassEntity",
                "MassSpawner",
                "MassCommon",
                "MassEntityDebugger",
                "MassMovement",
                "MassGameplayDebug",
                "MassSignals",
                "MassCrowd",
                "MassActors",
                "MassRepresentation",
                "MassReplication",
                "MassNavigation",
                "NetCore",
                "AIModule",
                "GeometryCore",
                "Chaos",
                "PhysicsCore",
                "ChaosCore",
                "ChaosSolverEngine"
            });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

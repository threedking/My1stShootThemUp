// My 1st Unreal Shoot Them Up Game

using UnrealBuildTool;

public class My1stShootThemUp : ModuleRules
{
	public My1stShootThemUp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"Niagara", 
			"PhysicsCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange( new string[] { 
			"My1stShootThemUp/Public/Player", 
			"My1stShootThemUp/Public/Components", 
			"My1stShootThemUp/Public/Dev",
			"My1stShootThemUp/Public/Weapon",
			"My1stShootThemUp/Public/UI",
			"My1stShootThemUp/Public/Animations",
			"My1stShootThemUp/Public/Pickups",
			"My1stShootThemUp/Public/Weapon/Components",
		} );

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

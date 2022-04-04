// My 1st Unreal Shoot Them Up Game

using UnrealBuildTool;
using System.Collections.Generic;

public class My1stShootThemUpTarget : TargetRules
{
	public My1stShootThemUpTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "My1stShootThemUp" } );
	}
}

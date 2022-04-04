// My 1st Unreal Shoot Them Up Game

using UnrealBuildTool;
using System.Collections.Generic;

public class My1stShootThemUpEditorTarget : TargetRules
{
	public My1stShootThemUpEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "My1stShootThemUp" } );
	}
}

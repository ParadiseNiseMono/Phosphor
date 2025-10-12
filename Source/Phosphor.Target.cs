// copyright Paradise_NiseMono

using UnrealBuildTool;
using System.Collections.Generic;

public class PhosphorTarget : TargetRules
{
	public PhosphorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Phosphor" } );
	}
}

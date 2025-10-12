// copyright Paradise_NiseMono

using UnrealBuildTool;
using System.Collections.Generic;

public class PhosphorEditorTarget : TargetRules
{
	public PhosphorEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Phosphor" } );
	}
}

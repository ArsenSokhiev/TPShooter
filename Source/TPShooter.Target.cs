// Third Player Shooter, All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class TPShooterTarget : TargetRules
{
	public TPShooterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "TPShooter" } );
	}
}

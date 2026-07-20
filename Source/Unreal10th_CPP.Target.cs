// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Unreal10th_CPPTarget : TargetRules
{
	public Unreal10th_CPPTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "Unreal10th_CPP" } );
	}
}

// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 ScaledDamage=Damage.GetValueAtLevel(Level);
	
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const float ScaledDebuffChance = DebuffChance.GetValueAtLevel(Level);
	const float ScaledDebuffDamage = DebuffDamage.GetValueAtLevel(Level);
	const float ScaledDebuffDuration = DebuffDuration.GetValueAtLevel(Level);
	const float ScaledDebuffFrequency = DebuffFrequency.GetValueAtLevel(Level);
	if (Level==1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>ELECTROCUTE</>\n\n"

			//Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost:</><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Fire a lightning bolt to deal continuous damage to the enemy.</>\n\n"
			"<Default>Damage dealt every 0.1 seconds:</><Damage>%d</>\n\n"
			"<Default>There is a</><Damage>%.1f</><Default>chance of causing a paralyzing effect.</>\n\n"
			"<Default>Duration:</><Damage>%.1f</><Default>seconds</>\n\n"
			)
			
			//Values
			, Level, ManaCost, Cooldown, ScaledDamage, ScaledDebuffChance, ScaledDebuffDuration);
	}
	else
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>ELECTROCUTE</>\n\n"

			//Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost:</><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Shoots %f lightning that deals continuous damage to an equal number of enemies as the number of lightning bolts</>\n\n"
			"<Default>Damage dealt every 0.1 seconds:</><Damage>%d</>\n\n"
			"<Default>There is a</><Damage>%.1f</><Default>chance of causing a paralyzing effect.</>\n\n"
			"<Default>Duration:</><Damage>%.1f</><Default>seconds</>\n\n"
			)
			
			//Values
			, Level, ManaCost, Cooldown,FMath::Min(Level, MaxNumShockTargets), ScaledDamage, ScaledDebuffChance, ScaledDebuffDuration);
	}
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage=Damage.GetValueAtLevel(Level);
	
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const float ScaledDebuffChance = DebuffChance.GetValueAtLevel(Level);
	const float ScaledDebuffDamage = DebuffDamage.GetValueAtLevel(Level);
	const float ScaledDebuffDuration = DebuffDuration.GetValueAtLevel(Level);
	const float ScaledDebuffFrequency = DebuffFrequency.GetValueAtLevel(Level);
	
	return FString::Printf(TEXT(
			//Title
			"<Title>NEXT LEVEL</>\n\n"

			//Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost:</><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Shoots %f lightning that deals continuous damage to an equal number of enemies as the number of lightning bolts</>\n\n"
			"<Default>Damage dealt every 0.1 seconds:</><Damage>%d</>\n\n"
			"<Default>There is a</><Damage>%.1f</><Default>chance of causing a paralyzing effect.</>\n\n"
			"<Default>Duration:</><Damage>%.1f</><Default>seconds</>\n\n"
			)
			
			//Values
			, Level, ManaCost, Cooldown,FMath::Min(Level, MaxNumShockTargets), ScaledDamage, ScaledDebuffChance, ScaledDebuffDuration);
}

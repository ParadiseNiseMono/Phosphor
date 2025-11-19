// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/PhosphorFireBlast.h"

FString UPhosphorFireBlast::GetDescription(int32 Level)
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
			"<Title>FIRE BOLT</>\n\n"

			//Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost:</><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Launches %d</>\n\n"
			"<Default>Fire balls in all direction,each coming back and</>"
			"<Default>exploding upon return, causing</>"
			"<Damage>%d</><Default> radial fire damage with </>\n\n"
			"<Default>There is</><Damage>%.1f</><Default>chance of causing a burning effect.</>\n\n"
			"<Default>After burning, it deals</><Damage>%.1f</><Default>fire damage per second.</>\n\n"
			"<Default>Each time it deals</><Damage>%.1f</><Default>of damage.</>\n\n"
			"<Default>Duration</><Damage>%.1f</><Default>seconds</>\n\n"
			)
			
			//Values
			, Level, ManaCost, Cooldown, NumFireBalls, ScaledDamage, ScaledDebuffChance, ScaledDebuffFrequency, ScaledDebuffDamage, ScaledDebuffDuration);
}

FString UPhosphorFireBlast::GetNextLevelDescription(int32 Level)
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
			"<Default>Launches %d</>\n\n"
			"<Default>Fire balls in all direction,each coming back and</>"
			"<Default>exploding upon return, causing</>"
			"<Damage>%d</><Default> radial fire damage with </>\n\n"
			"<Default>There is</><Damage>%.1f</><Default>chance of causing a burning effect.</>\n\n"
			"<Default>After burning, it deals</><Damage>%.1f</><Default>fire damage per second.</>\n\n"
			"<Default>Each time it deals</><Damage>%.1f</><Default>of damage.</>\n\n"
			"<Default>Duration</><Damage>%.1f</><Default>seconds</>\n\n"
			)
			
			//Values
			, Level, ManaCost, Cooldown, NumFireBalls, ScaledDamage, ScaledDebuffChance, ScaledDebuffFrequency, ScaledDebuffDamage, ScaledDebuffDuration);
}

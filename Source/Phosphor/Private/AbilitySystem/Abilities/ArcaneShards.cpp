// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	const int32 ScaledDamage=Damage.GetValueAtLevel(Level);
	
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level==1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>ARCANE SHARDS</>\n\n"

			//Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost:</><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Generate a ring of arcane shards</>\n\n"
			"<Default>Damage caused at the center of the Shards: </><Damage>%d</>\n\n"
			"<Default>Damage decreases with distance.</>"
			)
			
			//Values
			, Level, ManaCost, Cooldown, ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>ARCANE SHARDS</>\n\n"

			//Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost:</><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Summon %d Arcane Shards</>\n\n"
			"<Default>Damage caused at the center of the Shards: </><Damage>%d</>\n\n"
			"<Default>Damage decreases with distance.</>"
			)
			
			//Values
			, Level, ManaCost, Cooldown,FMath::Min(Level, MaxNumShards), ScaledDamage);
	}
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage=Damage.GetValueAtLevel(Level);
	
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			//Title
			"<Title>NEXT LEVEL</>\n\n"

			//Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost:</><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Summon %d Arcane Shards</>\n\n"
			"<Default>Damage caused at the center of the Shards: </><Damage>%d</>\n\n"
			"<Default>Damage decreases with distance.</>"
			)
			
			//Values
			, Level, ManaCost, Cooldown,FMath::Min(Level, MaxNumShards), ScaledDamage);
}

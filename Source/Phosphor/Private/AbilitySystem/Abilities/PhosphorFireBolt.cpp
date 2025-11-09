// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/PhosphorFireBolt.h"

#include "PhosphorGameplayTags.h"

FString UPhosphorFireBolt::GetDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FPhosphorGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level==1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>FIRE BOLT</>\n\n"

			//Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost:</><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Shot a Fire Blot</>\n\n"
			"<Default>Each bolt can cause </><Damage>%d</><Default> damage</>\n\n"
			"<Default>Fire Damage with a chance to burn</>"
			)
			
			//Values
			, Level, ManaCost, Cooldown, Damage);
	}
	else
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>FIRE BOLT</>\n\n"

			//Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Mana Cost:</><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Shot %d Fire Blots</>\n\n"
			"<Default>Each bolt can cause </><Damage>%d</><Default> damage</>\n\n"
			"<Default>Fire Damage with a chance to burn</>"
			)
			
			//Values
			, Level, ManaCost, Cooldown,FMath::Min(Level, NumProjectiles), Damage);
	}
}

FString UPhosphorFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FPhosphorGameplayTags::Get().Damage_Fire);
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
			"<Default>Shot %d Fire Blots</>\n\n"
			"<Default>Each bolt can cause </><Damage>%d</><Default> damage</>\n\n"
			"<Default>Fire Damage with a chance to burn</>"
			)
			
			//Values
			, Level, ManaCost, Cooldown,FMath::Min(Level, NumProjectiles), Damage);
}

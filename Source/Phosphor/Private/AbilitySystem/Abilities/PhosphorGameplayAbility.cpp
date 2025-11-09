// copyright Paradise_NiseMono


#include "AbilitySystem/Abilities/PhosphorGameplayAbility.h"

#include "AbilitySystem/PhosphorAttributeSet.h"

FString UPhosphorGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"),L"Default Ability Name - LoremIpsum  LoremIpsum  LoremIpsum  LoremIpsum  LoremIpsum  LoremIpsum  LoremIpsum  LoremIpsum  LoremIpsum  LoremIpsum  LoremIpsum  LoremIpsum ",Level);
}

FString UPhosphorGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level:</><Level>%d</> \n<Default>Causes more damage</>"), Level);
}

FString UPhosphorGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell locked until Level: </><Level>%d</>"), Level);
}

float UPhosphorGameplayAbility::GetManaCost(const int32 Level) const
{
	float ManaCost = 0.0f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Modifier : CostEffect->Modifiers)
		{
			if (Modifier.Attribute == UPhosphorAttributeSet::GetManaAttribute())
			{
				Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UPhosphorGameplayAbility::GetCooldown(const int32 Level) const
{
	float Cooldown = 0.0f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(Level, Cooldown);
	}
	return Cooldown;
}

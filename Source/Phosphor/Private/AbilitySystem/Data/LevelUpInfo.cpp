// copyright Paradise_NiseMono


#include "AbilitySystem/Data/LevelUpInfo.h"

#include "Phosphor/PhosphorLogChannels.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	bool bSearching=true;
	while (bSearching)
	{
		//LevelUpInfo[1]=Level 1 Info
		//LevelUpInfo[2]=Level 2 Info
		if (LevelUpInformation.Num()-1<=Level) return Level;

		if (XP >= LevelUpInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	/*
	int Level = 1;

	for (Level; Level <= LevelUpInformation.Num(); Level++)
	{
		int32 Requirement = LevelUpInformation[Level].LevelUpRequirement;
		if (Requirement >= XP)
			return Level;
	}

	return Level;
	*/

	
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		if (LevelUpInformation.Num() - 1 <= Level) return Level;

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

// Copyright Andrei Bondarenko 2023


#include "Types/FPSTypes.h"

TMap<FName, FText> FMapOptions::GetLocalizedModeNames()
{
	static TMap<FName, FText> ModeNames;

	if (ModeNames.IsEmpty())
	{
		ModeNames.Add({"Deathmatch", NSLOCTEXT("MapModes", "DeathmatchMode", "Deathmatch")});
		ModeNames.Add({"Co-op", NSLOCTEXT("MapModes", "CoopMode", "Co-op")});
	}

	return ModeNames;
}

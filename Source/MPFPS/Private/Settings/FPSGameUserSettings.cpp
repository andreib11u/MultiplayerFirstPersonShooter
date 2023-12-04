// Copyright Andrei Bondarenko 2023


#include "Settings/FPSGameUserSettings.h"

UFPSGameUserSettings* UFPSGameUserSettings::GetFPSGameUserSettings()
{
	return Cast<UFPSGameUserSettings>(UGameUserSettings::GetGameUserSettings());
}

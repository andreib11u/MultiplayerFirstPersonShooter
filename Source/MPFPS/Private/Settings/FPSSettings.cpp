// Copyright Andrei Bondarenko 2023

#include "Settings/FPSSettings.h"

UFPSSettings::UFPSSettings()
{
	using TA = ETeamAttitude::Type;
	TeamAttitudes = {
		{ TA::Friendly, TA::Neutral, TA::Neutral }, // Neutral
		{ TA::Neutral, TA::Friendly, TA::Hostile }, // Player
		{ TA::Neutral, TA::Hostile, TA::Friendly }	// Zombies
	};
}

const UFPSSettings* UFPSSettings::Get()
{
	return GetDefault<UFPSSettings>();
}

ETeamAttitude::Type UFPSSettings::GetAttitude(const FGenericTeamId Of, const FGenericTeamId Towards)
{
	const TArray<FTeamAttitude>& TeamAttitudes = Get()->TeamAttitudes;
	const bool bOfValid = TeamAttitudes.IsValidIndex(Of.GetId());
	const bool bTowardsValid = TeamAttitudes.IsValidIndex(Towards.GetId());

	if (bOfValid && bTowardsValid)
	{
		const TArray<TEnumAsByte<ETeamAttitude::Type>>& Attitudes = TeamAttitudes[Of.GetId()].Attitude;
		if (Attitudes.IsValidIndex(Towards.GetId()))
		{
			return Attitudes[Towards.GetId()];
		}
	}
	return ETeamAttitude::Neutral;
}

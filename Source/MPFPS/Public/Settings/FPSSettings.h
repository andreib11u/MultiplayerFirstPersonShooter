// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Types/FPSTypes.h"
#include "FPSSettings.generated.h"

/**
 *
 */
UCLASS(Config = Game, DefaultConfig)
class MPFPS_API UFPSSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UFPSSettings();

	UPROPERTY(Category = "Teams", EditAnywhere, BlueprintReadOnly, Config)
	TArray<FTeamAttitude> TeamAttitudes;

	static const UFPSSettings* Get();

	UFUNCTION(Category = "Teams", BlueprintPure)
	static ETeamAttitude::Type GetAttitude(FGenericTeamId Of, FGenericTeamId Towards);
};

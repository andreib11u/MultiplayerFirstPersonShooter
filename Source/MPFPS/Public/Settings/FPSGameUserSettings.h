// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FPSGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class MPFPS_API UFPSGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:

	static UFPSGameUserSettings* GetFPSGameUserSettings();
};

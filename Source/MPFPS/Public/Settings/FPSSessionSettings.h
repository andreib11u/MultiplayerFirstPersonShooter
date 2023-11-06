// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FPSSessionSettings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class MPFPS_API UFPSSessionSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float FriendlyFireDamageMultiplier = 0.1f;
};

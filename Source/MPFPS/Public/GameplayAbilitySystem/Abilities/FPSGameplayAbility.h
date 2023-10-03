// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FPSGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MPFPS_API UFPSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	void ReplicatedEndAbility();

	bool BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately);
};

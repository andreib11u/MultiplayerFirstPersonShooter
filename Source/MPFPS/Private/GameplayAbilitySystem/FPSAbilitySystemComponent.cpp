// Copyright Andrei Bondarenko 2023


#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"

bool UFPSAbilitySystemComponent::BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool bEndAbilityImmediately)
{
	bool AbilityActivated = false;
	if (InAbilityHandle.IsValid())
	{
		FScopedServerAbilityRPCBatcher AbilityRPCBatcher(this, InAbilityHandle);
		AbilityActivated = TryActivateAbility(InAbilityHandle, true);

		if (bEndAbilityImmediately)
		{
			FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(InAbilityHandle);
			if (AbilitySpec)
			{
				UFPSGameplayAbility* FPSAbility = Cast<UFPSGameplayAbility>(AbilitySpec->GetPrimaryInstance());
				FPSAbility->ReplicatedEndAbility();
			}
		}

		return AbilityActivated;
	}

	return AbilityActivated;
}

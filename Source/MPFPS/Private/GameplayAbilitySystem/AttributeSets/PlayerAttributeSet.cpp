// Copyright Andrei Bondarenko 2023


#include "GameplayAbilitySystem/AttributeSets/PlayerAttributeSet.h"

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetSpreadAttribute())
	{
		NewValue = FMath::Clamp(NewValue, MinSpread, MaxSpread);
	}
}

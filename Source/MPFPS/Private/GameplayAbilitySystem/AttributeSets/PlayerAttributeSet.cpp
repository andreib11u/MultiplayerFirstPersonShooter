// Copyright Andrei Bondarenko 2023


#include "GameplayAbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "Net/UnrealNetwork.h"

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetSpreadAttribute())
	{
		NewValue = FMath::Clamp(NewValue, MinSpread, MaxSpread);
	}

	if (Attribute == GetMoneyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, TNumericLimits<float>::Max());
	}
}

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Money, COND_OwnerOnly, REPNOTIFY_Always);
}

void UPlayerAttributeSet::OnRep_Money(const FGameplayAttributeData& OldMoney)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Money, OldMoney);
}

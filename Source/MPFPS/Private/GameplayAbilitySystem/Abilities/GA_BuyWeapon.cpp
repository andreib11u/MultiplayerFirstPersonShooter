// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GA_BuyWeapon.h"
#include "AbilitySystemComponent.h"
#include "Characters/ShootingCharacter.h"
#include "GameplayAbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "Weapons/EquipmentComponent.h"

bool UGA_BuyWeapon::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							  FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ActorInfo)
	{
		const UAttributeSet* AttributeSet = ActorInfo->AbilitySystemComponent->GetAttributeSet(UPlayerAttributeSet::StaticClass());
		if (!AttributeSet)
		{
			return false;
		}

		const UPlayerAttributeSet* PlayerAttributeSet = CastChecked<UPlayerAttributeSet>(AttributeSet);

		return PlayerAttributeSet->GetMoney() >= WeaponCost;
	}

	return false;
}

bool UGA_BuyWeapon::CommitAbilityCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									  const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags)
{
	if (ActorInfo)
	{
		const UAttributeSet* AttributeSet = ActorInfo->AbilitySystemComponent->GetAttributeSet(UPlayerAttributeSet::StaticClass());
		if (!AttributeSet)
		{
			return false;
		}

		const UPlayerAttributeSet* PlayerAttributeSet = CastChecked<UPlayerAttributeSet>(AttributeSet);

		bool bHasEnoughMoney = PlayerAttributeSet->GetMoney() >= WeaponCost;

		if (bHasEnoughMoney && ActorInfo->IsNetAuthority())
		{
			FGameplayEffectSpecHandle AddMoneyEffectSpec = MakeOutgoingGameplayEffectSpec(AddMoneyEffect);

			AddMoneyEffectSpec.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Value.Money"), -WeaponCost);
			FActiveGameplayEffectHandle Spec = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, AddMoneyEffectSpec);
		}

		return bHasEnoughMoney;
	}

	return false;
}

// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GA_BuyWeapon.h"

#include "AbilitySystemComponent.h"
#include "Characters/PlayerCharacter.h"
#include "GameplayAbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "Weapons/EquipmentComponent.h"

bool UGA_BuyWeapon::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	const UAttributeSet* AttributeSet = ActorInfo->AbilitySystemComponent->GetAttributeSet(UPlayerAttributeSet::StaticClass());
	if (!AttributeSet)
	{
		return false;
	}

	const UPlayerAttributeSet* PlayerAttributeSet = CastChecked<UPlayerAttributeSet>(AttributeSet);

	return PlayerAttributeSet->GetMoney() > WeaponCost;
}

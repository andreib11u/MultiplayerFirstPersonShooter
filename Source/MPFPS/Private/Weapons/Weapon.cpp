// Copyright Andrei Bondarenko 2023

#include "Weapons/Weapon.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"
#include "Weapons/EquipmentComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

void UWeapon::OnEquip(UEquipmentComponent* EquipmentComponent)
{
	EquipmentComponent->SetAmmoFrom(this);

	if (EquipmentComponent->GetOwnerRole() == ROLE_Authority)
	{
		if (!ReloadAbility)
		{
			UE_LOG(LogWeapon, Error, TEXT("%s doesn't have ReloadAbility set"), *GetName())
			return;
		}

		UAbilitySystemComponent* AbilitySystemComponent = EquipmentComponent->GetAbilitySystemComponent();
		if (AbilitySystemComponent)
		{
			auto ReloadAbilityCDO = Cast<UFPSGameplayAbility>(ReloadAbility->GetDefaultObject());

			FGameplayAbilitySpec Spec =
				FGameplayAbilitySpec(ReloadAbility, 1, static_cast<int32>(ReloadAbilityCDO->GetAbilityInput()), EquipmentComponent->GetOwner());
			if (ensure(AbilitySystemComponent))
			{
				AbilitySystemComponent->GiveAbility(Spec);
			}
		}
	}
}

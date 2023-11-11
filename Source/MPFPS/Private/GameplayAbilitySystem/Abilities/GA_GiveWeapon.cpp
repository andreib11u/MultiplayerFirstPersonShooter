// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GA_GiveWeapon.h"
#include "Characters/PlayerCharacter.h"
#include "Weapons/EquipmentComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGiveWeaponAbility, All, All);

void UGA_GiveWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (auto PlayerCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor))
	{
		PlayerCharacter->GetEquipmentComponent()->AddWeapon(Weapon);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

		// todo: animation and tags
	}
	else
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		UE_LOG(LogGiveWeaponAbility, Error, TEXT("GiveWeapon ability tried to activate on non-PlayerCharacter actor"))
	}
}

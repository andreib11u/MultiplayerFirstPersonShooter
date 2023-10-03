// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GA_Shoot.h"

#include "AbilitySystemComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Weapons/WeaponComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameplayAbilitySystem/Abilities/GameplayAbility_FireOnce.h"

DEFINE_LOG_CATEGORY_STATIC(LogShootAbility, All, All);

void UGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (auto PlayerCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor))
	{
		auto Weapon = PlayerCharacter->GetWeaponComponent()->GetCurrentWeapon();
		if (!Weapon)
		{
			return;
		}

		FGameplayAbilitySpec* SingleShotAbilitySpec = GetAbilitySystemComponentFromActorInfo()->FindAbilitySpecFromClass(SingleShotAbilityClass);
		if (!SingleShotAbilitySpec)
		{
			UE_LOG(LogShootAbility, Error, TEXT("AbilityOwner doesn't have %s ability"), *SingleShotAbilityClass->GetName());
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
			return;
		}

		SingleShotAbilityInstance = Cast<UGameplayAbility_FireOnce>(SingleShotAbilitySpec->GetPrimaryInstance());
		SingleShotAbilitySpecHandle = SingleShotAbilitySpec->Handle;

		if (Weapon->FireMode.MatchesTag(SemiAutoTag))
		{
			BatchRPCTryActivateAbility(SingleShotAbilitySpecHandle, true);
		}

		if (Weapon->FireMode.MatchesTag(FullAutoTag))
		{
			const bool bAbilityActivated = BatchRPCTryActivateAbility(SingleShotAbilitySpecHandle, false);
			if (!bAbilityActivated)
			{
				UE_LOG(LogShootAbility, Warning, TEXT("Ability couldn't batch activate"));
				CancelAbility(Handle, ActorInfo, ActivationInfo, true);
				return;
			}

			UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
			if (!WaitInputReleaseTask)
			{
				UE_LOG(LogShootAbility, Warning, TEXT("Couldn't create WaitInputRelease task. Ability is canceled"));
				CancelAbility(SingleShotAbilitySpecHandle, ActorInfo, ActivationInfo, true);
				CancelAbility(Handle, ActorInfo, ActivationInfo, true);
				return;
			}

			WaitInputReleaseTask->OnRelease.AddDynamic(this, &UGA_Shoot::OnInputRelease);
			WaitInputReleaseTask->Activate();

			UAbilityTask_WaitDelay* WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, ShotCooldown);
			if (!WaitDelayTask)
			{
				UE_LOG(LogShootAbility, Warning, TEXT("Couldn't create WaitDelay task. Ability is canceled"));
				CancelAbility(SingleShotAbilitySpecHandle, ActorInfo, ActivationInfo, true);
				CancelAbility(Handle, ActorInfo, ActivationInfo, true);
				return;
			}

			WaitDelayTask->OnFinish.AddDynamic(this, &UGA_Shoot::OnShotCooldownExpired);
			WaitDelayTask->Activate();
		}
		else
		{
			UE_LOG(LogShootAbility, Error, TEXT("FireMode for weapon %s is not set"), *Weapon->GetName())
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		}
	}
}

void UGA_Shoot::OnInputRelease(float TimeHeld)
{
	EndBothAbilities();
}

void UGA_Shoot::EndBothAbilities()
{
	SingleShotAbilityInstance->CancelAbility(SingleShotAbilitySpecHandle, SingleShotAbilityInstance->GetCurrentActorInfo(),
											 SingleShotAbilityInstance->GetCurrentActivationInfo(), true);

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_Shoot::OnShotCooldownExpired()
{
	if (CheckCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo()))
	{
		SingleShotAbilityInstance->FireShot();
		UAbilityTask_WaitDelay* WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, ShotCooldown);

		WaitDelayTask->OnFinish.AddDynamic(this, &UGA_Shoot::OnShotCooldownExpired);
		WaitDelayTask->Activate();
	}
	else
	{
		EndBothAbilities();
	}
}

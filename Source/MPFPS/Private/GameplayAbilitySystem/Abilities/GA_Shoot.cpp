// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GA_Shoot.h"
#include "AbilitySystemComponent.h"
#include "Characters/ShootingCharacter.h"
#include "Weapons/EquipmentComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameplayAbilitySystem/Abilities/GA_FireOnce.h"

DEFINE_LOG_CATEGORY_STATIC(LogShootAbility, All, All);

void UGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (auto PlayerCharacter = Cast<AShootingCharacter>(ActorInfo->AvatarActor))
	{
		auto Weapon = Cast<UWeapon>(PlayerCharacter->GetEquipmentComponent()->GetCurrentItem());
		if (!Weapon)
		{
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
			return;
		}

		const float ShotTime = GetWorld()->GetTimeSeconds();
		const float ShotTimeDifference = ShotTime - LastShotTime;
		LastShotTime = ShotTime;

		if (ShotTimeDifference < Weapon->WeaponStats.ShotCooldown)
		{
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
			return;
		}

		FGameplayAbilitySpec* SingleShotAbilitySpec = GetAbilitySystemComponentFromActorInfo()->FindAbilitySpecFromClass(SingleShotAbilityClass);
		if (!SingleShotAbilitySpec)
		{
			UE_LOG(LogShootAbility, Error, TEXT("AbilityOwner doesn't have SingleShotAbility set"));
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
			return;
		}

		SingleShotAbilityInstance = Cast<UGA_FireOnce>(SingleShotAbilitySpec->GetPrimaryInstance());
		SingleShotAbilitySpecHandle = SingleShotAbilitySpec->Handle;

		if (Weapon->GetFireModeTag().MatchesTag(SemiAutoTag))
		{
			BatchRPCTryActivateAbility(SingleShotAbilitySpecHandle, true);
		}

		if (Weapon->GetFireModeTag().MatchesTag(FullAutoTag))
		{
			const bool bAbilityActivated = BatchRPCTryActivateAbility(SingleShotAbilitySpecHandle, false);
			if (!bAbilityActivated)
			{
				UE_LOG(LogShootAbility, Warning, TEXT("Ability couldn't batch activate"));
				CancelAbility(Handle, ActorInfo, ActivationInfo, true);
				return;
			}

			WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
			if (!WaitInputReleaseTask)
			{
				UE_LOG(LogShootAbility, Warning, TEXT("Couldn't create WaitInputRelease task. Ability is canceled"));
				CancelAbility(SingleShotAbilitySpecHandle, ActorInfo, ActivationInfo, true);
				CancelAbility(Handle, ActorInfo, ActivationInfo, true);
				return;
			}

			WaitInputReleaseTask->OnRelease.AddDynamic(this, &UGA_Shoot::OnInputRelease);
			WaitInputReleaseTask->Activate();

			WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, ShotCooldown);
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

bool UGA_Shoot::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
						  FGameplayTagContainer* OptionalRelevantTags) const
{
	if (auto EquipmentComponent = ActorInfo->AvatarActor->FindComponentByClass<UEquipmentComponent>())
	{
		return EquipmentComponent->GetCurrentClipAmmo() > 0.f;
	}

	return false;
}

void UGA_Shoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Shoot::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

}

void UGA_Shoot::OnInputRelease(float TimeHeld)
{
	EndShootAbilities();
}

void UGA_Shoot::EndShootAbilities()
{
	if (SingleShotAbilityInstance)
	{
		SingleShotAbilityInstance->CancelAbility(SingleShotAbilitySpecHandle, SingleShotAbilityInstance->GetCurrentActorInfo(),
												 SingleShotAbilityInstance->GetCurrentActivationInfo(), true);
	}

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_Shoot::OnShotCooldownExpired()
{
	if (CheckCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), nullptr))
	{
		SingleShotAbilityInstance->FireShot();
		WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, ShotCooldown);

		WaitDelayTask->OnFinish.AddDynamic(this, &UGA_Shoot::OnShotCooldownExpired);
		WaitDelayTask->Activate();
	}
	else
	{
		EndShootAbilities();
	}
}

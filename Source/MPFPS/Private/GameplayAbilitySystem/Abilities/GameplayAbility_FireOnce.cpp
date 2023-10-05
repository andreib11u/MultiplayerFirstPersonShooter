// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GameplayAbility_FireOnce.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Characters/PlayerCharacter.h"
#include "GameplayAbilitySystem/TargetActors/TargetActor_LineTrace.h"
#include "Weapons/Weapon.h"
#include "Weapons/EquipmentComponent.h"

void UGameplayAbility_FireOnce::FireShot()
{
	// ammo
	CommitAbilityCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), nullptr);

	// line trace
	WaitTargetDataTask =
		UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::Instant, ATargetActor_LineTrace::StaticClass());

	WaitTargetDataTask->ValidData.AddUniqueDynamic(this, &UGameplayAbility_FireOnce::OnValidDataAcquired);
	WaitTargetDataTask->ReadyForActivation();

	AGameplayAbilityTargetActor* SpawnedActor;
	WaitTargetDataTask->BeginSpawningActor(this, ATargetActor_LineTrace::StaticClass(), SpawnedActor);
	WaitTargetDataTask->FinishSpawningActor(this, SpawnedActor);
}

void UGameplayAbility_FireOnce::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	FireShot();
}

bool UGameplayAbility_FireOnce::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	// todo: cache weapon pointer
	auto PlayerCharacter = Cast<APlayerCharacter>(GetCurrentActorInfo()->AvatarActor);
	auto Weapon = Cast<UWeapon>(PlayerCharacter->GetWeaponComponent()->GetCurrentItem());
	return Weapon->GetCurrentClipAmmo() > 0.f;
}

bool UGameplayAbility_FireOnce::CommitAbilityCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags)
{
	auto PlayerCharacter = Cast<APlayerCharacter>(GetCurrentActorInfo()->AvatarActor);
	auto Weapon = Cast<UWeapon>(PlayerCharacter->GetWeaponComponent()->GetCurrentItem());
	if (Weapon)
	{
		Weapon->SpendAmmo(1.f);
	}

	return true;
}

void UGameplayAbility_FireOnce::OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data)
{
	const FGameplayAbilityTargetData* DataPtr = Data.Get(0);
	if (DataPtr)
	{
		FGameplayCueParameters Parameters;
		Parameters.EffectCauser = GetAvatarActorFromActorInfo();
		Parameters.Location = DataPtr->GetEndPoint();

		const FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(DamageEffect);

		auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(DataPtr->GetHitResult()->GetActor());
		if (AbilitySystemInterface)
		{
			AbilitySystemInterface->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
		}

		Parameters.EffectContext = EffectSpec.Data.Get()->GetEffectContext();
		Parameters.EffectContext.AddHitResult(*DataPtr->GetHitResult());

		GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(ShotGameplayCue, Parameters);
	}
}

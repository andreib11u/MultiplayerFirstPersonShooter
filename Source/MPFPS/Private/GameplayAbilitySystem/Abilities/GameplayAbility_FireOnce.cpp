// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GameplayAbility_FireOnce.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Characters/PlayerCharacter.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_ServerWaitForClientData.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_WaitTargetDataUsingActor.h"
#include "GameplayAbilitySystem/TargetActors/TargetActor_LineTrace.h"
#include "Weapons/Weapon.h"
#include "Weapons/EquipmentComponent.h"

void UGameplayAbility_FireOnce::FireShot()
{
	if (GetActorInfo().IsLocallyControlledPlayer())
	{
		const float ShotTime = GetWorld()->GetTimeSeconds();
		const float ShotTimeDifference = ShotTime - LastShotTime;

		if (ShotTimeDifference > TimeBetweenShots)
		{
			if (!TargetActor)
			{
				SpawnTargetActor();
			}

			WaitTargetDataTask = UAbilityTask_WaitTargetDataUsingActor::WaitTargetDataWithReusableActor(
				this, NAME_None, EGameplayTargetingConfirmation::Instant, TargetActor, true);

			WaitTargetDataTask->ValidData.AddDynamic(this, &UGameplayAbility_FireOnce::OnValidDataAcquired);
			WaitTargetDataTask->Activate();
		}
	}
}

void UGameplayAbility_FireOnce::SpawnTargetActor()
{
	UAbilityTask_WaitTargetData* SpawnTargetActorTask =
		UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::Instant, ATargetActor_LineTrace::StaticClass());

	AGameplayAbilityTargetActor* SpawnedActor;
	SpawnTargetActorTask->BeginSpawningActor(this, ATargetActor_LineTrace::StaticClass(), SpawnedActor);
	SpawnTargetActorTask->FinishSpawningActor(this, SpawnedActor);

	TargetActor = SpawnedActor;
}

void UGameplayAbility_FireOnce::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
												const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	SpawnTargetActor();

	ServerWaitForClientDataTask = UAbilityTask_ServerWaitForClientData::ServerWaitForClientTargetData(this, NAME_None, false);
	ServerWaitForClientDataTask->ValidData.AddDynamic(this, &UGameplayAbility_FireOnce::OnValidDataAcquired);
	ServerWaitForClientDataTask->Activate();

	FireShot();
}

bool UGameplayAbility_FireOnce::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
										  FGameplayTagContainer* OptionalRelevantTags) const
{
	if (auto EquipmentComponent = GetCurrentActorInfo()->AvatarActor->FindComponentByClass<UEquipmentComponent>())
	{
		return EquipmentComponent->GetCurrentClipAmmo() > 0.f;
	}

	return false;
}

bool UGameplayAbility_FireOnce::CommitAbilityCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
												  const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags)
{
	if (auto EquipmentComponent = GetCurrentActorInfo()->AvatarActor->FindComponentByClass<UEquipmentComponent>())
	{
		EquipmentComponent->SpendAmmo(1.f);
	}

	return true;
}

void UGameplayAbility_FireOnce::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ServerWaitForClientDataTask)
	{
		ServerWaitForClientDataTask->EndTask();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_FireOnce::OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data)
{
	CommitAbilityCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), nullptr);

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

		LastShotTime = GetWorld()->GetTimeSeconds();
	}
}

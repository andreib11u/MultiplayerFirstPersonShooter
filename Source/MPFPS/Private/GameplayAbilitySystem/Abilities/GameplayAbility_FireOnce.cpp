// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GameplayAbility_FireOnce.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Characters/PlayerCharacter.h"
#include "GameplayAbilitySystem/TargetActors/TargetActor_LineTrace.h"
#include "Weapons/Weapon.h"
#include "Weapons/WeaponComponent.h"

void UGameplayAbility_FireOnce::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
												const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("FireOnce Activate"))

	// ammo
	auto PlayerCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor);
	if (!(PlayerCharacter && PlayerCharacter->GetWeaponComponent()->GetCurrentWeapon()))
	{
		return;
	}

	PlayerCharacter->GetWeaponComponent()->GetCurrentWeapon()->CurrentClipAmmo--;

	// line trace
	WaitTargetDataTask =
		UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::Instant, ATargetActor_LineTrace::StaticClass());

	WaitTargetDataTask->ValidData.AddDynamic(this, &UGameplayAbility_FireOnce::OnValidData);
	WaitTargetDataTask->ReadyForActivation();

	AGameplayAbilityTargetActor* SpawnedActor;
	WaitTargetDataTask->BeginSpawningActor(this, ATargetActor_LineTrace::StaticClass(), SpawnedActor);
	WaitTargetDataTask->FinishSpawningActor(this, SpawnedActor);
}

void UGameplayAbility_FireOnce::OnValidData(const FGameplayAbilityTargetDataHandle& Data)
{
	WaitTargetDataTask->ValidData.RemoveDynamic(this, &UGameplayAbility_FireOnce::OnValidData);

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

		GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Shooting.BulletImpact"),
																	 Parameters);
	}

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

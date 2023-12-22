// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GA_Revive.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Camera/CameraComponent.h"
#include "Characters/ShootingCharacter.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_ServerWaitForClientData.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_WaitTargetDataUsingActor.h"
#include "GameplayAbilitySystem/TargetActors/TargetActor_LineTrace.h"
#include "Types/CollisionTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogReviveAbility, All, All);

void UGA_Revive::ConfigureTargetActor()
{
	auto LineTraceActor = Cast<ATargetActor_LineTrace>(TargetActor);
	if (LineTraceActor)
	{
		const FVector CameraLocation = ReviverCharacter->GetFirstPersonCamera()->GetComponentLocation();
		const FVector EndTrace =
			CameraLocation + ReviverCharacter->GetFirstPersonCamera()->GetForwardVector() * ReviverCharacter->GetInteractionLength();

		LineTraceActor->Configure(CameraLocation, EndTrace, INTERACTION_TRACE_COLLISION);
	}
}

void UGA_Revive::SpawnTargetActor()
{
	if (!TargetActor)
	{
		UAbilityTask_WaitTargetData* SpawnTargetActorTask = UAbilityTask_WaitTargetData::WaitTargetData(
			this, NAME_None, EGameplayTargetingConfirmation::Instant, ATargetActor_LineTrace::StaticClass());

		SpawnTargetActorTask->BeginSpawningActor(this, ATargetActor_LineTrace::StaticClass(), TargetActor);

		ConfigureTargetActor();

		SpawnTargetActorTask->FinishSpawningActor(this, TargetActor);
	}
}

void UGA_Revive::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ReviverCharacter = Cast<AShootingCharacter>(ActorInfo->AvatarActor);
	if (!ReviverCharacter.IsValid())
	{
		UE_LOG(LogReviveAbility, Error, TEXT("Revive ability tried to activate on non-PlayerCharacter actor"));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	ServerWaitForClientDataTask = UAbilityTask_ServerWaitForClientData::ServerWaitForClientTargetData(this, NAME_None, true);
	ServerWaitForClientDataTask->ValidData.AddDynamic(this, &UGA_Revive::OnValidDataAcquired);
	ServerWaitForClientDataTask->ReadyForActivation();

	if (ActorInfo->IsLocallyControlled())
	{
		if (!TargetActor)
		{
			SpawnTargetActor();
		}

		ConfigureTargetActor();

		WaitTargetDataTask = UAbilityTask_WaitTargetDataUsingActor::WaitTargetDataWithReusableActor(
			this, NAME_None, EGameplayTargetingConfirmation::Instant, TargetActor, true);

		WaitTargetDataTask->ValidData.AddDynamic(this, &UGA_Revive::OnValidDataAcquired);
		WaitTargetDataTask->ReadyForActivation();
	}
}

void UGA_Revive::OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data)
{
	if (Data.IsValid(0))
	{
		const FGameplayAbilityTargetData* TargetData = Data.Get(0);

		if (auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(TargetData->GetHitResult()->GetActor()))
		{
			if (!HealEffect)
			{
				UE_LOG(LogReviveAbility, Warning, TEXT("Revive ability doesn't have ReviveEffect set"))
				return;
			}

			if (!HealEffect)
			{
				UE_LOG(LogReviveAbility, Warning, TEXT("Revive ability doesn't have HealEffect set"))
				return;
			}

			TargetAbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();

			FGameplayEffectSpecHandle ReviveEffectSpec = MakeOutgoingGameplayEffectSpec(ReviveEffect);
			ActiveReviveEffectHandle = TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*ReviveEffectSpec.Data);

			FGameplayEffectSpecHandle HealEffectSpec = MakeOutgoingGameplayEffectSpec(HealEffect);
			HealEffectSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Value.Health"), HealValue);
			TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*HealEffectSpec.Data);
		}
	}

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}

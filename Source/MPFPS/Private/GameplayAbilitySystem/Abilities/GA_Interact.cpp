// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GA_Interact.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Camera/CameraComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Components/InteractionComponent.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_WaitTargetDataUsingActor.h"
#include "GameplayAbilitySystem/TargetActors/TargetActor_LineTrace.h"
#include "Types/CollisionTypes.h"
#include "Weapons/EquipmentComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogInteractAbility, All, All);

void UGA_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	auto PlayerCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor);
	if (!PlayerCharacter)
	{
		UE_LOG(LogInteractAbility, Error, TEXT("Interact ability tried to activate on non-PlayerCharacter actor"));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	if (!TargetActor)
	{
		SpawnTargetActor();
	}

	auto LineTraceActor = Cast<ATargetActor_LineTrace>(TargetActor);
	if (LineTraceActor)
	{
		const FVector CameraLocation = PlayerCharacter->GetFirstPersonCamera()->GetComponentLocation();
		const FVector EndTrace = CameraLocation + PlayerCharacter->GetFirstPersonCamera()->GetForwardVector() * InteractionLength;

		LineTraceActor->Configure(CameraLocation, EndTrace, INTERACTION_TRACE_COLLISION);
	}

	WaitTargetDataTask = UAbilityTask_WaitTargetDataUsingActor::WaitTargetDataWithReusableActor(
		this, NAME_None, EGameplayTargetingConfirmation::Instant, TargetActor, true);

	WaitTargetDataTask->ValidData.AddDynamic(this, &UGA_Interact::OnValidDataAcquired);
	WaitTargetDataTask->ReadyForActivation();
}

void UGA_Interact::SpawnTargetActor()
{
	if (!TargetActor)
	{
		AGameplayAbilityTargetActor* SpawnedActor = nullptr;

		UAbilityTask_WaitTargetData* SpawnTargetActorTask = UAbilityTask_WaitTargetData::WaitTargetData(
			this, NAME_None, EGameplayTargetingConfirmation::Instant, ATargetActor_LineTrace::StaticClass());

		SpawnTargetActorTask->BeginSpawningActor(this, ATargetActor_LineTrace::StaticClass(), SpawnedActor);
		SpawnTargetActorTask->FinishSpawningActor(this, SpawnedActor);

		TargetActor = SpawnedActor;
	}
}

void UGA_Interact::OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data)
{
	const FGameplayAbilityTargetData* DataPtr = Data.Get(0);
	if (DataPtr && DataPtr->GetHitResult())
	{
		if (auto InteractionComponent = Cast<UInteractionComponent>(DataPtr->GetHitResult()->GetComponent()))
		{
			TSubclassOf<UFPSGameplayAbility> InteractionAbility = InteractionComponent->GetInteractionAbility();
			if (InteractionAbility)
			{
				FGameplayAbilitySpec InteractionAbilitySpec = GetAbilitySystemComponentFromActorInfo()->BuildAbilitySpecFromClass(InteractionAbility);
				GetAbilitySystemComponentFromActorInfo()->GiveAbilityAndActivateOnce(InteractionAbilitySpec);

				EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
			}
		}
	}

	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

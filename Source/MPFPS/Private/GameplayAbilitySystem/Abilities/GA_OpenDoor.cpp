// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GA_OpenDoor.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Actors/Door.h"
#include "Camera/CameraComponent.h"
#include "Characters/ShootingCharacter.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_ServerWaitForClientData.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_WaitTargetDataUsingActor.h"
#include "GameplayAbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "GameplayAbilitySystem/TargetActors/TargetActor_LineTrace.h"
#include "Types/CollisionTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogOpenDoorAbility, All, All);

void UGA_OpenDoor::ConfigureTargetActor()
{
	auto LineTraceActor = Cast<ATargetActor_LineTrace>(TargetActor);
	if (LineTraceActor)
	{
		const FVector CameraLocation = PlayerCharacter->GetFirstPersonCamera()->GetComponentLocation();
		const FVector EndTrace =
			CameraLocation + PlayerCharacter->GetFirstPersonCamera()->GetForwardVector() * PlayerCharacter->GetInteractionLength();

		LineTraceActor->Configure(CameraLocation, EndTrace, INTERACTION_TRACE_COLLISION);
	}
}

void UGA_OpenDoor::SpawnTargetActor()
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

void UGA_OpenDoor::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	PlayerCharacter = Cast<AShootingCharacter>(ActorInfo->AvatarActor);
	if (!PlayerCharacter.IsValid())
	{
		UE_LOG(LogOpenDoorAbility, Error, TEXT("%s ability tried to activate on non-PlayerCharacter actor"), *GetName());
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	ServerWaitForClientDataTask = UAbilityTask_ServerWaitForClientData::ServerWaitForClientTargetData(this, NAME_None, true);
	ServerWaitForClientDataTask->ValidData.AddDynamic(this, &UGA_OpenDoor::OnValidDataAcquired);
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

		WaitTargetDataTask->ValidData.AddDynamic(this, &UGA_OpenDoor::OnValidDataAcquired);
		WaitTargetDataTask->ReadyForActivation();
	}
}

void UGA_OpenDoor::OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data)
{
	if (Data.IsValid(0))
	{
		const FGameplayAbilityTargetData* TargetData = Data.Get(0);

		if (auto Door = Cast<ADoor>(TargetData->GetHitResult()->GetActor()))
		{
			Door->Open();
		}
	}

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}

bool UGA_OpenDoor::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							 FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ActorInfo)
	{
		const UAttributeSet* AttributeSet = ActorInfo->AbilitySystemComponent->GetAttributeSet(UPlayerAttributeSet::StaticClass());
		if (!AttributeSet)
		{
			return false;
		}

		const UPlayerAttributeSet* PlayerAttributeSet = CastChecked<UPlayerAttributeSet>(AttributeSet);

		return PlayerAttributeSet->GetMoney() >= Cost;
	}

	return false;
}

bool UGA_OpenDoor::CommitAbilityCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									 const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags)
{
	if (ActorInfo)
	{
		const UAttributeSet* AttributeSet = ActorInfo->AbilitySystemComponent->GetAttributeSet(UPlayerAttributeSet::StaticClass());
		if (!AttributeSet)
		{
			return false;
		}

		const UPlayerAttributeSet* PlayerAttributeSet = CastChecked<UPlayerAttributeSet>(AttributeSet);

		bool bHasEnoughMoney = PlayerAttributeSet->GetMoney() >= Cost;

		if (bHasEnoughMoney && ActorInfo->IsNetAuthority())
		{
			FGameplayEffectSpecHandle AddMoneyEffectSpec = MakeOutgoingGameplayEffectSpec(AddMoneyEffect);

			AddMoneyEffectSpec.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Value.Money"), -Cost);
			FActiveGameplayEffectHandle Spec = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, AddMoneyEffectSpec);
		}

		return bHasEnoughMoney;
	}

	return false;
}

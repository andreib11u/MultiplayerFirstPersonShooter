// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_ServerWaitForClientData.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"

UAbilityTask_ServerWaitForClientData::UAbilityTask_ServerWaitForClientData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UAbilityTask_ServerWaitForClientData* UAbilityTask_ServerWaitForClientData::ServerWaitForClientTargetData(UGameplayAbility* OwningAbility,
																										  FName TaskInstanceName, bool TriggerOnce)
{
	UAbilityTask_ServerWaitForClientData* MyObj = NewAbilityTask<UAbilityTask_ServerWaitForClientData>(OwningAbility, TaskInstanceName);
	MyObj->bTriggerOnce = TriggerOnce;
	return MyObj;
}

void UAbilityTask_ServerWaitForClientData::Activate()
{
	if (!Ability || !Ability->GetCurrentActorInfo()->IsNetAuthority())
	{
		return;
	}

	FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
	FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
	AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey)
		.AddUObject(this, &UAbilityTask_ServerWaitForClientData::OnTargetDataReplicatedCallback);
}

void UAbilityTask_ServerWaitForClientData::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag)
{
	FGameplayAbilityTargetDataHandle MutableData = Data;
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(MutableData);
	}

	if (bTriggerOnce)
	{
		EndTask();
	}
}

void UAbilityTask_ServerWaitForClientData::OnDestroy(bool AbilityEnded)
{
	if (AbilitySystemComponent.IsValid())
	{
		FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).RemoveAll(this);
	}

	Super::OnDestroy(AbilityEnded);
}
// Copyright Andrei Bondarenko 2023


#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_Tick.h"

UAbilityTask_Tick::UAbilityTask_Tick(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

UAbilityTask_Tick* UAbilityTask_Tick::AbilityTaskTick(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	UAbilityTask_Tick* MyObj = NewAbilityTask<UAbilityTask_Tick>(OwningAbility, TaskInstanceName);
	return MyObj;
}

void UAbilityTask_Tick::Activate()
{
	Super::Activate();
}

void UAbilityTask_Tick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTick.Broadcast(DeltaTime);
	}
}

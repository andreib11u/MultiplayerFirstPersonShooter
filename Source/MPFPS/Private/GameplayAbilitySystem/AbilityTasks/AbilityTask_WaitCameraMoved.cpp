// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_WaitCameraMoved.h"
#include "Characters/PlayerCharacter.h"

UAbilityTask_WaitCameraMoved::UAbilityTask_WaitCameraMoved()
{
	bTickingTask = true;
}

UAbilityTask_WaitCameraMoved* UAbilityTask_WaitCameraMoved::WaitCameraMoved(UGameplayAbility* OwningAbility, FName TaskInstanceName,
																			APlayerCharacter* PlayerCharacter)
{
	UAbilityTask_WaitCameraMoved* NewTask = NewAbilityTask<UAbilityTask_WaitCameraMoved>(OwningAbility, TaskInstanceName);
	NewTask->Character = PlayerCharacter;

	return NewTask;
}

void UAbilityTask_WaitCameraMoved::Activate()
{
	Character->OnCameraFullyMoved.AddDynamic(this, &UAbilityTask_WaitCameraMoved::CameraMoved);
}

void UAbilityTask_WaitCameraMoved::CameraMoved()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCameraMoved.Broadcast();
	}

	EndTask();
}

// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/AbilityTasks/WaitChangeFOVTask.h"

UWaitChangeFOVTask::UWaitChangeFOVTask()
{
	bTickingTask = true;
	bIsActive = false;
}

UWaitChangeFOVTask* UWaitChangeFOVTask::WaitChangeFOV(UGameplayAbility* OwningAbility, FName TaskInstanceName,
													  class UCameraComponent* InCameraComponent, float InTargetFOV, float InInterpSpeed)
{
	check(InCameraComponent);

	UWaitChangeFOVTask* NewTask = NewAbilityTask<UWaitChangeFOVTask>(OwningAbility, TaskInstanceName);

	NewTask->CameraComponent = InCameraComponent;
	NewTask->TargetFOV = InTargetFOV;
	NewTask->InterpSpeed = InInterpSpeed;
	NewTask->CurrentFOV = InCameraComponent->FieldOfView;

	check(InInterpSpeed != 0.f);

	return NewTask;
}

void UWaitChangeFOVTask::Activate()
{
	bIsActive = true;
}

void UWaitChangeFOVTask::TickTask(float DeltaTime)
{
	if (!bIsActive)
	{
		return;
	}

	Super::TickTask(DeltaTime);

	if (CameraComponent)
	{
		CurrentFOV = FMath::FInterpConstantTo(CurrentFOV, TargetFOV, DeltaTime, InterpSpeed);
		CameraComponent->SetFieldOfView(CurrentFOV);

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnChangeFOVTick.Broadcast(DeltaTime, CurrentFOV);
		}

		if (FMath::IsNearlyEqual(CurrentFOV, TargetFOV))
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnTargetFOVReached.Broadcast();
			}

			bIsActive = false;
			EndTask();
		}
	}
	else
	{
		bIsActive = false;
		EndTask();
	}
}
// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_WaitChangeFOV.h"
#include "Camera/CameraComponent.h"

UAbilityTask_WaitChangeFOV::UAbilityTask_WaitChangeFOV()
{
	bTickingTask = true;
	bIsActive = false;
}

UAbilityTask_WaitChangeFOV* UAbilityTask_WaitChangeFOV::WaitChangeFOV(UGameplayAbility* OwningAbility, FName TaskInstanceName,
																	  class UCameraComponent* InCameraComponent, float InTargetFOV,
																	  float InInterpSpeed)
{
	check(InCameraComponent);

	UAbilityTask_WaitChangeFOV* NewTask = NewAbilityTask<UAbilityTask_WaitChangeFOV>(OwningAbility, TaskInstanceName);

	NewTask->CameraComponent = InCameraComponent;
	NewTask->TargetFOV = InTargetFOV;
	NewTask->InterpSpeed = InInterpSpeed;
	NewTask->CurrentFOV = InCameraComponent->FieldOfView;

	check(InInterpSpeed != 0.f);

	return NewTask;
}

void UAbilityTask_WaitChangeFOV::Activate()
{
	bIsActive = true;
}

void UAbilityTask_WaitChangeFOV::TickTask(float DeltaTime)
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
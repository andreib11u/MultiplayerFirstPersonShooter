// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitChangeFOV.generated.h"

class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetFOVReached);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeFOVTick, float, DeltaTime, float, CurrentFOV);

/**
 *
 */
UCLASS()
class MPFPS_API UAbilityTask_WaitChangeFOV : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAbilityTask_WaitChangeFOV();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
			  meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitChangeFOV* WaitChangeFOV(UGameplayAbility* OwningAbility, FName TaskInstanceName, UCameraComponent* InCameraComponent,
													 float InTargetFOV, float InInterpSpeed = 5.f);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnTargetFOVReached OnTargetFOVReached;
	UPROPERTY(BlueprintAssignable)
	FOnChangeFOVTick OnChangeFOVTick;

private:
	bool bIsActive;

	UPROPERTY()
	UCameraComponent* CameraComponent;

	float TargetFOV;
	float CurrentFOV;
	float InterpSpeed;
};

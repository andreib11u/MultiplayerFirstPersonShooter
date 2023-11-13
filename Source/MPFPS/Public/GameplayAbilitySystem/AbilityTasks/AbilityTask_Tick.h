// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_Tick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTickTaskDelegate, float, DeltaTime);

/**
 *
 */
UCLASS()
class MPFPS_API UAbilityTask_Tick : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnTickTaskDelegate OnTick;

	UAbilityTask_Tick(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
			  meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_Tick* AbilityTaskTick(UGameplayAbility* OwningAbility, FName TaskInstanceName);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

private:
	float TaskDeltaTime;
};

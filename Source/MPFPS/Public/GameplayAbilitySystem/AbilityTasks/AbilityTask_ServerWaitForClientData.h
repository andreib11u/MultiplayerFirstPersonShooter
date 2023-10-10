// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "AbilityTask_ServerWaitForClientData.generated.h"

/**
 *
 */
UCLASS()
class MPFPS_API UAbilityTask_ServerWaitForClientData : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAbilityTask_ServerWaitForClientData(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FWaitTargetDataDelegate ValidData;

	UFUNCTION(BlueprintCallable,
			  meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParms = "Instigator"),
			  Category = "Ability|Tasks")
	static UAbilityTask_ServerWaitForClientData* ServerWaitForClientTargetData(UGameplayAbility* OwningAbility, FName TaskInstanceName,
																			   bool TriggerOnce);

	virtual void Activate() override;

	UFUNCTION()
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag);

protected:
	virtual void OnDestroy(bool AbilityEnded) override;

	bool bTriggerOnce;
};

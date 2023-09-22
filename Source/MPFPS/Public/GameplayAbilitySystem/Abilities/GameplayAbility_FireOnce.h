// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "GameplayAbility_FireOnce.generated.h"

/**
 *
 */
UCLASS()
class MPFPS_API UGameplayAbility_FireOnce : public UGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnValidData(const FGameplayAbilityTargetDataHandle& Data);

	UPROPERTY()
	UAbilityTask_WaitTargetData* WaitTargetDataTask;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageEffect;
};

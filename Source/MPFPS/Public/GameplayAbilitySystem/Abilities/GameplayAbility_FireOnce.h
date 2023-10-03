// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "FPSGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "GameplayAbility_FireOnce.generated.h"

/**
 *
 */
UCLASS()
class MPFPS_API UGameplayAbility_FireOnce : public UFPSGameplayAbility
{
	GENERATED_BODY()
public:
	void FireShot();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data);

	UPROPERTY()
	UAbilityTask_WaitTargetData* WaitTargetDataTask;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditAnywhere, meta = (GameplayTagFilter = "GameplayCue"))
	FGameplayTag ShotGameplayCue = FGameplayTag::RequestGameplayTag("GameplayCue.Shooting.BulletImpact");
};

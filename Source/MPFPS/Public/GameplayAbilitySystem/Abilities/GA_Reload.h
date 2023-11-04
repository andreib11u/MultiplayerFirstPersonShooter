// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_PlayMontageForMesh.h"
#include "GA_Reload.generated.h"

/**
 *
 */
UCLASS()
class MPFPS_API UGA_Reload : public UFPSGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
									FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							  const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY(EditAnywhere)
	UAnimMontage* ThirdPersonReloadMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* FirstPersonReloadMontage;

	UPROPERTY(EditAnywhere)
	FGameplayTag ReloadEventTag;
	UPROPERTY()
	UAbilityTask_PlayMontageForMesh* PlayFirstPersonMontage;

	UFUNCTION()
	void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
	void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
	void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_WaitTargetDataUsingActor.h"
#include "GA_Interact.generated.h"

class AGameplayAbilityTargetActor;
/**
 *
 */
UCLASS()
class MPFPS_API UGA_Interact : public UFPSGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditAnywhere)
	float InteractionLength = 100.f; // todo: replace with value from a character;

	UPROPERTY()
	UAbilityTask_WaitTargetDataUsingActor* WaitTargetDataTask;
	UPROPERTY()
	AGameplayAbilityTargetActor* TargetActor;

	void SpawnTargetActor();
	UFUNCTION()
	void OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data);
};

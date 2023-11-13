// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Characters/PlayerCharacter.h"
#include "Components/InteractionComponent.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_Tick.h"
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

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

private:
	UPROPERTY(EditAnywhere)
	float InteractionLength = 100.f; // todo: replace with value from a character;
	float TimeActive = 0.f;

	UPROPERTY()
	UAbilityTask_WaitTargetDataUsingActor* WaitTargetDataTask;
	UPROPERTY()
	AGameplayAbilityTargetActor* TargetActor;

	TWeakObjectPtr<APlayerCharacter> PlayerCharacter;
	UPROPERTY()
	UAbilityTask_Tick* TickTask;

	TWeakObjectPtr<UInteractionComponent> InteractionComponent;
	UPROPERTY()
	UAbilityTask_WaitInputRelease* WaitInputReleaseTask;

	void SpawnTargetActor();
	UFUNCTION()
	void OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data);
	UFUNCTION()
	void InteractTick(float DeltaTime);
	UFUNCTION()
	void OnInputRelease(float TimeHeld);
	void ActivateInteractionAbility();
	UFUNCTION()
	void OnFirstValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data);
};

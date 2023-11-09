// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Characters/PlayerCharacter.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"
#include "GameplayAbilitySystem/AbilityTasks/WaitChangeFOVTask.h"
#include "GA_AimDownSights.generated.h"

/**
 *
 */
UCLASS()
class MPFPS_API UGA_AimDownSights : public UFPSGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							  const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> AimingEffect;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> AimingRemovalEffect;

	UPROPERTY(EditAnywhere, meta = (Units = "Degrees"))
	float TargetFOV = 60.f;
	float InitialFOV;

	UPROPERTY(EditAnywhere)
	float InterpSpeed = 45.f;

	UPROPERTY()
	UWaitChangeFOVTask* WaitChangeFOVTask;
	UPROPERTY()
	UAbilityTask_WaitInputRelease* WaitInputReleaseTask;
	UPROPERTY()
	APlayerCharacter* OwnerCharacter;
	UPROPERTY()
	FActiveGameplayEffectHandle ActiveAimingEffectSpec;

	UFUNCTION()
	void OnTargetFOVReached();
	UFUNCTION()
	void ChangeFOVTick(float DeltaTime, float CurrentFOV);
	UFUNCTION()
	void OnInitialFOVReached();
	UFUNCTION()
	void OnInputRelease(float TimeHeld);
};

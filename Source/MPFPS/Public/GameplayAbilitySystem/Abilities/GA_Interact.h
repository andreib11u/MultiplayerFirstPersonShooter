// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"
#include "UI/HUDWidget.h"
#include "UI/HUD/InteractionWidget.h"
#include "GA_Interact.generated.h"

class UInteractionComponent;
class UAbilityTask_WaitInputRelease;
class UAbilityTask_Tick;
class AShootingCharacter;
class UAbilityTask_WaitTargetDataUsingActor;
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
	float TimeActive = 0.f;
	float InteractionProgress = 0.f;

	UPROPERTY()
	UAbilityTask_WaitTargetDataUsingActor* WaitTargetDataTask;
	UPROPERTY()
	AGameplayAbilityTargetActor* TargetActor;

	TWeakObjectPtr<AShootingCharacter> PlayerCharacter;
	UPROPERTY()
	UAbilityTask_Tick* TickTask;

	TWeakObjectPtr<UInteractionComponent> InteractionComponent;
	UPROPERTY()
	UAbilityTask_WaitInputRelease* WaitInputReleaseTask;

	TWeakObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;
	TWeakObjectPtr<UInteractionWidget> InteractionWidget;

	void SpawnTargetActor();
	UFUNCTION()
	void OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data);
	void SetInteractionProgress(float CurrentInteractionProgress);
	UFUNCTION()
	void InteractTick(float DeltaTime);
	UFUNCTION()
	void OnInputRelease(float TimeHeld);
	UFUNCTION()
	void OnFirstValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data);

	void ActivateInteractionAbility();
	void FindInteractionWidget();
};

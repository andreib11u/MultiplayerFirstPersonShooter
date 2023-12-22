// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_ServerWaitForClientData.h"
#include "GA_Revive.generated.h"

class AShootingCharacter;
class UAbilityTask_WaitCameraMoved;
class UAbilityTask_WaitTargetDataUsingActor;

/**
 * Revive a downed player
 */
UCLASS()
class MPFPS_API UGA_Revive : public UFPSGameplayAbility
{
	GENERATED_BODY()
public:
	void ConfigureTargetActor();
	void SpawnTargetActor();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> ReviveEffect;

	UPROPERTY()
	UAbilityTask_WaitTargetDataUsingActor* WaitTargetDataTask;

	TWeakObjectPtr<AShootingCharacter> ReviverCharacter;
	UPROPERTY()
	UAbilityTask_WaitCameraMoved* WaitCameraMovedTask;
	TWeakObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;
	FActiveGameplayEffectHandle ActiveReviveEffectHandle;
	FDelegateHandle OnRecoveredDelegateHandle;
	UPROPERTY()
	UAbilityTask_ServerWaitForClientData* ServerWaitForClientDataTask;
	UPROPERTY()
	AGameplayAbilityTargetActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere)
	float HealValue = 60.f;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> HealEffect;

	UFUNCTION()
	void OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data);
};

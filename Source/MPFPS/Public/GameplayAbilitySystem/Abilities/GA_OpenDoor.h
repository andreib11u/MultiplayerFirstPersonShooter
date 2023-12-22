// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Characters/ShootingCharacter.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_ServerWaitForClientData.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_WaitTargetDataUsingActor.h"
#include "GA_OpenDoor.generated.h"

class AGameplayAbilityTargetActor;
/**
 *
 */
UCLASS()
class MPFPS_API UGA_OpenDoor : public UFPSGameplayAbility
{
	GENERATED_BODY()
public:
	void ConfigureTargetActor();
	void SpawnTargetActor();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
						   FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual bool CommitAbilityCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								   const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags) override;

private:
	UPROPERTY()
	AGameplayAbilityTargetActor* TargetActor = nullptr;

	TWeakObjectPtr<AShootingCharacter> PlayerCharacter;
	UPROPERTY()
	UAbilityTask_ServerWaitForClientData* ServerWaitForClientDataTask;
	UPROPERTY()
	UAbilityTask_WaitTargetDataUsingActor* WaitTargetDataTask;

	UPROPERTY(EditAnywhere)
	float Cost = 100.f;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> AddMoneyEffect;

	UFUNCTION()
	void OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data);
};

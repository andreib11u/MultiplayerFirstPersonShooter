// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_PlayMontageForMesh.h"
#include "GA_GiveWeapon.generated.h"

class UWeapon;
/**
 *
 */
UCLASS()
class MPFPS_API UGA_GiveWeapon : public UFPSGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWeapon> Weapon;
	UPROPERTY(EditAnywhere)
	UAnimMontage* ThirdPersonMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* FirstPersonMontage;

	UPROPERTY()
	UAbilityTask_PlayMontageForMesh* PlayFirstPersonMontageTask;

	UFUNCTION()
	void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
};


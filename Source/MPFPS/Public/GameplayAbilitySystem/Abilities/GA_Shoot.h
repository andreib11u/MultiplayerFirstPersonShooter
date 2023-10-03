// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "FPSGameplayAbility.h"
#include "GA_Shoot.generated.h"

/**
 *
 */
UCLASS()
class MPFPS_API UGA_Shoot : public UFPSGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditAnywhere, meta = (GameplayTagFilter = "Weapon.FireMode"))
	FGameplayTag SemiAutoTag = FGameplayTag::RequestGameplayTag("Weapon.FireMode.SemiAuto");
	UPROPERTY(EditAnywhere, meta = (GameplayTagFilter = "Weapon.FireMode"))
	FGameplayTag FullAutoTag = FGameplayTag::RequestGameplayTag("Weapon.FireMode.FullAuto");

	UPROPERTY(EditAnywhere)
	float ShotCooldown = 0.1f;

	UPROPERTY()
	class UGameplayAbility_FireOnce* SingleShotAbilityInstance;

	UFUNCTION()
	void OnInputRelease(float TimeHeld);
	UFUNCTION()
	void OnShotCooldownExpired();

	void EndBothAbilities();

	FGameplayAbilitySpecHandle SingleShotAbilitySpecHandle;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility_FireOnce> SingleShotAbilityClass;
};

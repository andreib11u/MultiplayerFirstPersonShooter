// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"
#include "GA_ZombieAttack.generated.h"

/**
 *
 */
UCLASS()
class MPFPS_API UGA_ZombieAttack : public UFPSGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_ZombieAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void Cancel();

private:
	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> AttackMontages;
	UPROPERTY(EditAnywhere)
	FGameplayTag AttackEventTag = FGameplayTag::RequestGameplayTag("Zombie.Event.Attack");
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float AttackSpeed = 1.f;

	UPROPERTY(EditAnywhere, meta = (Categories = "GameplayCue"))
	FGameplayTag GameplayCueTag;

	UFUNCTION()
	void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
	void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
	void OnAnimEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
	void OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data);


	// box collision config

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	UPROPERTY(EditAnywhere)
	FVector HalfSize = FVector{ 40.f, 30.f, 30.f };
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1"))
	float AttackRange = 50.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageEffect;
};

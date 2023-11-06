// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "BulletDamage.generated.h"

/**
 *
 */
UCLASS()
class MPFPS_API UBulletDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UBulletDamage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
										FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	void CalculateFriendlyFireDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec,
									 float& OutDamage) const;
};

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "PlayerAttributeSet.generated.h"

/**
 *
 */
UCLASS()
class MPFPS_API UPlayerAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()
public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Spread;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Spread)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Money)
	FGameplayAttributeData Money;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Money)

	UFUNCTION()
	void OnRep_Money(const FGameplayAttributeData& OldMoney);

	// Max/Min values

	float MinSpread = 0.f;
	float MaxSpread = 1.f;
};

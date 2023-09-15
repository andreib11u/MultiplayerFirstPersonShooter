// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FPSAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class MPFPS_API UFPSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	template<typename AttributeSet>
	const AttributeSet* GetAttributeSet(TSubclassOf<UAttributeSet> AttributeSetClass = AttributeSet::StaticClass());
};

template <typename AttributeSet>
const AttributeSet* UFPSAbilitySystemComponent::GetAttributeSet(TSubclassOf<UAttributeSet> AttributeSetClass)
{
	return Cast<AttributeSet>(UAbilitySystemComponent::GetAttributeSet(AttributeSetClass));
}
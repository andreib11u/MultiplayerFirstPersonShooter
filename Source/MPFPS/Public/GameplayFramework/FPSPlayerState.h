// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "FPSPlayerState.generated.h"

class UFPSAbilitySystemComponent;
class UBaseAttributeSet;
class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class MPFPS_API AFPSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AFPSPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFPSAbilitySystemComponent* GetFPSAbilitySystemComponent() const { return AbilitySystemComponent; }
	UBaseAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
	UFPSAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	UBaseAttributeSet* AttributeSet;

	void CurrentHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData);
};

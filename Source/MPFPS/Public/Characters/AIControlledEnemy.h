// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Characters/BaseCharacter.h"
#include "AIControlledEnemy.generated.h"

class UAttributeSet;
class UFPSAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class MPFPS_API AAIControlledEnemy : public ABaseCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAIControlledEnemy();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


protected:
	virtual void BeginPlay() override;


	/** Create in subclasses */
	UPROPERTY()
	UAttributeSet* DefaultAttributeSet;
private:
	UPROPERTY(EditAnywhere)
	UFPSAbilitySystemComponent* AbilitySystemComponent;

};

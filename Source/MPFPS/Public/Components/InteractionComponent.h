// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InteractionComponent.generated.h"

class UFPSGameplayAbility;
/**
 * Something that the player can interact with
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class MPFPS_API UInteractionComponent : public UBoxComponent
{
	GENERATED_BODY()
public:
	TSubclassOf<UFPSGameplayAbility> GetInteractionAbility() const { return InteractionAbility; }
	bool IsInstant() const { return bInstant; }

	float GetActivationTime() const { return ActivationTime; }
	FText GetInteractionText() const { return InteractionText; }

protected:
	virtual void PostInitProperties() override;

private:
	/** Executes on a player when interaction happens */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UFPSGameplayAbility> InteractionAbility;

	/** Whether to instantly activate InteractionAbility or after ActivationTime */
	UPROPERTY(EditAnywhere)
	bool bInstant;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bInstant", EditConditionHides, Units = "s", ClampMin = "0"))
	float ActivationTime = 1.f;

	/** Showed when the player is looking at this component */
	UPROPERTY(EditAnywhere)
	FText InteractionText;
};

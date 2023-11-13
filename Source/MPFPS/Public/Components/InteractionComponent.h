// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InteractionComponent.generated.h"

class UFPSGameplayAbility;
/**
 *
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

	float GetProgress() const { return InteractionProgress; }
	void SetInteractionProgress(float Progress) { InteractionProgress = Progress; }

protected:
	virtual void PostInitProperties() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UFPSGameplayAbility> InteractionAbility;

	/** Whether to instantly activate InteractionAbility on an interacting character or after ActivationTime */
	UPROPERTY(EditAnywhere)
	bool bInstant;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bInstant", EditConditionHides, Units = "s", ClampMin = "0"))
	float ActivationTime = 1.f;

	UPROPERTY(EditAnywhere)
	FText InteractionText;

	float InteractionProgress = 0.f;
};

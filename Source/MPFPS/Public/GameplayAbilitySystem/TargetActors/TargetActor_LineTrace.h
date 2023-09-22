// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TargetActor_LineTrace.generated.h"

/**
 *
 */
UCLASS()
class MPFPS_API ATargetActor_LineTrace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	ATargetActor_LineTrace();
	TArray<FHitResult> PerformTrace();

	/** Initialize and begin targeting logic  */
	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual bool IsConfirmTargetingAllowed() override;

	/** Requesting targeting data, but not necessarily stopping/destroying the task. Useful for external target data requests. */
	virtual void ConfirmTargetingAndContinue() override;

	/** Outside code is saying 'stop and just give me what you have.' Returns true if the ability accepts this and can be forgotten. */
	virtual void ConfirmTargeting() override;

	/** Outside code is saying 'stop everything and just forget about it' */
	virtual void CancelTargeting() override;

	virtual void BindToConfirmCancelInputs() override;

	virtual bool ShouldProduceTargetData() const override;

	/** Replicated target data was received from a client. Possibly sanitize/verify. return true if data is good and we should broadcast it as valid
	 * data. */
	virtual bool OnReplicatedTargetDataReceived(FGameplayAbilityTargetDataHandle& Data) const override;

	FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<FHitResult>& HitResults) const;

protected:
	virtual void BeginPlay() override;
};

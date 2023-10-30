// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Characters/AIControlledEnemy.h"
#include "Zombie.generated.h"

/**
 * 
 */
UCLASS()
class MPFPS_API AZombie : public AAIControlledEnemy
{
	GENERATED_BODY()
public:
	AZombie();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComponent;
	UPROPERTY(EditAnywhere, meta = (Units = "Seconds", ClampMin = "0"))
	float AudioInterval = 10.f;
	UPROPERTY(EditAnywhere, meta = (Units = "Seconds", ClampMin = "0"))
	float IntervalRandomDeviation = 5.f;

	FTimerHandle AudioTimerHandle;
	FRandomStream RandomStream;

	void PlaySound();
};

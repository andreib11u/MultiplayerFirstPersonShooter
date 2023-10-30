// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCue_ZombieHit.generated.h"

/**
 * 
 */
UCLASS()
class MPFPS_API UGameplayCue_ZombieHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

private:
	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere)
	USoundAttenuation* SoundAttenuation;
};

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCue_BulletImpact.generated.h"

/**
 * 
 */
UCLASS()
class MPFPS_API UGameplayCue_BulletImpact : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CameraShake;
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
	UParticleSystem* ShotTrail;
	UPROPERTY(EditAnywhere)
	USoundBase* ShotSound;
	UPROPERTY(EditAnywhere)
	USoundBase* HumanImpactSound;
	UPROPERTY(EditAnywhere)
	USoundBase* ObjectImpactSound;
	UPROPERTY(EditAnywhere)
	UParticleSystem* HumanImpactParticle;
	UPROPERTY(EditAnywhere)
	UParticleSystem* ObjectImpactParticle;
	UPROPERTY(EditAnywhere)
	USoundAttenuation* SoundAttenuation;
};

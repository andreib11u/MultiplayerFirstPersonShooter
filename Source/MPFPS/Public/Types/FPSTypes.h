// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "FPSTypes.generated.h"

USTRUCT()
struct FWeaponStats
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float MaxClipAmmo;

	UPROPERTY(EditAnywhere)
	float MaxReserveAmmo;

	UPROPERTY(EditAnywhere)
	float DamagePerBullet;
	UPROPERTY(EditAnywhere)
	float HeadshotDamage;
	UPROPERTY(EditAnywhere)
	float LegshotDamage;

	UPROPERTY(EditAnywhere)
	float ShotCooldown;

	UPROPERTY(EditAnywhere)
	float StandSpread = 0.1f;
	UPROPERTY(EditAnywhere)
	float WalkSpread = 0.5f;
	UPROPERTY(EditAnywhere)
	float SpreadDecay = 3.f;
};

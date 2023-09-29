// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract, EditInlineNew, DefaultToInstanced)
class MPFPS_API UWeapon : public UObject
{
	GENERATED_BODY()
public:

protected:
	virtual bool IsNameStableForNetworking() const override { return true; }
	virtual bool IsSupportedForNetworking() const override { return true; }
public:
	UPROPERTY(EditAnywhere)
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere)
	float MaxClipAmmo;
	UPROPERTY(EditAnywhere, Replicated)
	float CurrentClipAmmo;

	UPROPERTY(EditAnywhere)
	float MaxAmmo;
	UPROPERTY(EditAnywhere)
	float CurrentAmmo;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> ShootAbility;
};

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTags.h"
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
	float MaxReserveAmmo;
	UPROPERTY(EditAnywhere)
	float CurrentReserveAmmo;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> ShootAbility; //todo remove

	UPROPERTY(EditAnywhere)
	FName MuzzleSocket = "MuzzleFlashSocket";

	UPROPERTY(EditAnywhere, meta = (GameplayTagFilter = "Weapon.FireMode"))
	FGameplayTag FireMode;
};

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "EquippableItems/EquippableItem.h"
#include "Weapon.generated.h"

class UGameplayAbility;
/**
 *
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class MPFPS_API UWeapon : public UEquippableItem
{
	GENERATED_BODY()
public:
	FName GetMuzzleSocketName() const { return MuzzleSocket; }
	FGameplayTag GetFireModeTag() const { return FireMode; }

	virtual void OnEquip(UEquipmentComponent* EquipmentComponent) override;

protected:
	virtual bool IsNameStableForNetworking() const override { return true; } // todo: remove
	virtual bool IsSupportedForNetworking() const override { return true; }

public:
	UPROPERTY(EditAnywhere)
	float MaxClipAmmo;
	UPROPERTY(EditAnywhere)
	float CurrentClipAmmo;

	UPROPERTY(EditAnywhere)
	float MaxReserveAmmo;
	UPROPERTY(EditAnywhere)
	float CurrentReserveAmmo;

private:
	UPROPERTY(EditAnywhere)
	FName MuzzleSocket = "MuzzleFlashSocket";

	UPROPERTY(EditAnywhere, meta = (GameplayTagFilter = "Weapon.FireMode"))
	FGameplayTag FireMode;
};

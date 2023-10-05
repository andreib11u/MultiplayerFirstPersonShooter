// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "EquippableItems/EquippableItem.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentClipAmmoChanged, float, CurrentClipAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentReserveAmmoChanged, float, CurrentReserveAmmo);

class UGameplayAbility;
/**
 *
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class MPFPS_API UWeapon : public UEquippableItem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnCurrentClipAmmoChanged OnCurrentClipAmmoChanged;
	UPROPERTY(BlueprintAssignable)
	FOnCurrentReserveAmmoChanged OnCurrentReserveAmmoChanged;

	void SpendAmmo(float Ammo);
	void ReloadAmmo();

	void SetCurrentClipAmmo(float Ammo);
	void SetCurrentReserveAmmo(float Ammo);
	float GetCurrentClipAmmo() const { return CurrentClipAmmo; }
	float GetCurrentReserveAmmo() const { return CurrentReserveAmmo; }
	FName GetMuzzleSocketName() const { return MuzzleSocket; }
	FGameplayTag GetFireModeTag() const { return FireMode; }

protected:
	virtual bool IsNameStableForNetworking() const override { return true; } // todo: remove
	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void PostLoad() override;

private:
	UPROPERTY(EditAnywhere)
	float MaxClipAmmo;
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_CurrentClipAmmo)
	float CurrentClipAmmo;

	UPROPERTY(EditAnywhere)
	float MaxReserveAmmo;
	UPROPERTY(EditAnywhere)
	float CurrentReserveAmmo;

	UPROPERTY(EditAnywhere)
	FName MuzzleSocket = "MuzzleFlashSocket";

	UPROPERTY(EditAnywhere, meta = (GameplayTagFilter = "Weapon.FireMode"))
	FGameplayTag FireMode;

	UFUNCTION()
	void OnRep_CurrentClipAmmo();
};

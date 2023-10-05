// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UI/FPSUserWidget.h"
#include "AmmoWidget.generated.h"

class UWeapon;
class UTextBlock;
/**
 * 
 */
UCLASS()
class MPFPS_API UAmmoWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void Bind(UWeapon* Weapon);
	void Unbind();
	void SetAmmoText(float InCurrentAmmo, float InReserveAmmo);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoText;

	UFUNCTION()
	void OnReserveAmmoChanged(float CurrentReserveAmmo);
	UFUNCTION()
	void OnCurrentAmmoChanged(float CurrentClipAmmo);

	float CurrentAmmo;
	float ReserveAmmo;

	UPROPERTY()
	UWeapon* BoundWeapon; 
};

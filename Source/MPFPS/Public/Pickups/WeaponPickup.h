// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Pickups/Pickup.h"
#include "WeaponPickup.generated.h"

class UWeapon;
/**
 * 
 */
UCLASS()
class MPFPS_API AWeaponPickup : public APickup
{
	GENERATED_BODY()
public:
	virtual void OnPickup(APlayerCharacter* PlayerCharacter) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWeapon> WeaponClass;

};

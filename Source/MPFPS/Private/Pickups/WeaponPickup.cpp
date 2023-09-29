// Copyright Andrei Bondarenko 2023


#include "Pickups/WeaponPickup.h"
#include "Characters/PlayerCharacter.h"
#include "Weapons/WeaponComponent.h"

void AWeaponPickup::OnPickup(APlayerCharacter* PlayerCharacter)
{
	Super::OnPickup(PlayerCharacter);

	PlayerCharacter->GetWeaponComponent()->AddWeapon(WeaponClass);
}
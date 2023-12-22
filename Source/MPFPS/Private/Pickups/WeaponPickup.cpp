// Copyright Andrei Bondarenko 2023


#include "Pickups/WeaponPickup.h"
#include "Characters/ShootingCharacter.h"
#include "Weapons/EquipmentComponent.h"

void AWeaponPickup::OnPickup(AShootingCharacter* PlayerCharacter)
{
	Super::OnPickup(PlayerCharacter);

	PlayerCharacter->GetEquipmentComponent()->AddWeapon(WeaponClass);
}

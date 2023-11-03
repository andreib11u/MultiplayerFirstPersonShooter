// Copyright Andrei Bondarenko 2023


#include "Pickups/WeaponPickup.h"
#include "Characters/PlayerCharacter.h"
#include "Weapons/EquipmentComponent.h"

void AWeaponPickup::OnPickup(APlayerCharacter* PlayerCharacter)
{
	Super::OnPickup(PlayerCharacter);

	PlayerCharacter->GetEquipmentComponent()->AddWeapon(WeaponClass);
}

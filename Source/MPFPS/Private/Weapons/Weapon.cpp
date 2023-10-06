// Copyright Andrei Bondarenko 2023


#include "Weapons/Weapon.h"
#include "Weapons/EquipmentComponent.h"


void UWeapon::OnEquip(UEquipmentComponent* EquipmentComponent)
{
	EquipmentComponent->SetAmmoFrom(this);
}


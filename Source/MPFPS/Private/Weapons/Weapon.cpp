// Copyright Andrei Bondarenko 2023


#include "Weapons/Weapon.h"
#include "Net/UnrealNetwork.h"

void UWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UWeapon, CurrentClipAmmo, COND_OwnerOnly);
}

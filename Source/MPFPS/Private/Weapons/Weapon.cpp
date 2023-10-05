// Copyright Andrei Bondarenko 2023


#include "Weapons/Weapon.h"
#include "Net/UnrealNetwork.h"

void UWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UWeapon, CurrentClipAmmo, COND_OwnerOnly);
}

void UWeapon::SpendAmmo(float Ammo)
{
	check(CurrentClipAmmo >= Ammo);

	CurrentClipAmmo -= Ammo;
	OnCurrentClipAmmoChanged.Broadcast(CurrentClipAmmo);
}

void UWeapon::ReloadAmmo()
{
	check(CurrentReserveAmmo > 0.f);
	check(CurrentClipAmmo < MaxClipAmmo);

	const float AmmoNeededToFull = CurrentClipAmmo - MaxClipAmmo;
	if (AmmoNeededToFull >= CurrentReserveAmmo)
	{
		CurrentClipAmmo += CurrentReserveAmmo;
		CurrentReserveAmmo = 0.f;
	}
	else
	{
		CurrentClipAmmo += AmmoNeededToFull;
		CurrentReserveAmmo -= AmmoNeededToFull;
	}

	OnCurrentClipAmmoChanged.Broadcast(CurrentClipAmmo);
	OnCurrentReserveAmmoChanged.Broadcast(CurrentReserveAmmo);
}

void UWeapon::SetCurrentClipAmmo(float Ammo)
{
	CurrentClipAmmo = Ammo;
	OnCurrentClipAmmoChanged.Broadcast(CurrentClipAmmo);
}

void UWeapon::SetCurrentReserveAmmo(float Ammo)
{
	CurrentReserveAmmo = Ammo;
	OnCurrentReserveAmmoChanged.Broadcast(CurrentReserveAmmo);
}

void UWeapon::PostLoad()
{
	Super::PostLoad();

	CurrentClipAmmo = MaxClipAmmo;
	CurrentReserveAmmo = MaxReserveAmmo;
}

void UWeapon::OnRep_CurrentClipAmmo()
{
	OnCurrentClipAmmoChanged.Broadcast(CurrentClipAmmo);
}

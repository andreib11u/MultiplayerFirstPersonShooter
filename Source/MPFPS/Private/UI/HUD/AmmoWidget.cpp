// Copyright Andrei Bondarenko 2023


#include "UI/HUD/AmmoWidget.h"
#include "Components/TextBlock.h"
#include "Weapons/Weapon.h"

void UAmmoWidget::Bind(UWeapon* Weapon)
{
	BoundWeapon = Weapon;
	BoundWeapon->OnCurrentReserveAmmoChanged.AddDynamic(this, &UAmmoWidget::OnReserveAmmoChanged);
	BoundWeapon->OnCurrentClipAmmoChanged.AddDynamic(this, &UAmmoWidget::OnCurrentAmmoChanged);

	OnCurrentAmmoChanged(Weapon->GetCurrentClipAmmo());
	OnReserveAmmoChanged(Weapon->GetCurrentReserveAmmo());
}

void UAmmoWidget::Unbind()
{
	if (BoundWeapon)
	{
		BoundWeapon->OnCurrentReserveAmmoChanged.RemoveDynamic(this, &UAmmoWidget::OnReserveAmmoChanged);
		BoundWeapon->OnCurrentClipAmmoChanged.RemoveDynamic(this, &UAmmoWidget::OnCurrentAmmoChanged);
	}

	OnCurrentAmmoChanged(0.f);
	OnReserveAmmoChanged(0.f);
}

void UAmmoWidget::SetAmmoText(float InCurrentAmmo, float InReserveAmmo)
{
	const FText Text = FText::Format(NSLOCTEXT("UI", "AmmoBar", "{0}/{1}"), InCurrentAmmo, InReserveAmmo);
	AmmoText->SetText(Text);
}

void UAmmoWidget::OnReserveAmmoChanged(float InReserveAmmo)
{
	ReserveAmmo = InReserveAmmo;
	SetAmmoText(CurrentAmmo, ReserveAmmo);
}

void UAmmoWidget::OnCurrentAmmoChanged(float CurrentClipAmmo)
{
	CurrentAmmo = CurrentClipAmmo;
	SetAmmoText(CurrentAmmo, ReserveAmmo);
}

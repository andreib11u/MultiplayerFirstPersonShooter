// Copyright Andrei Bondarenko 2023

#include "UI/HUD/AmmoWidget.h"
#include "Components/TextBlock.h"
#include "Weapons/EquipmentComponent.h"

void UAmmoWidget::Bind(UEquipmentComponent* EquipmentComponent)
{
	BoundEquipment = EquipmentComponent;
	BoundEquipment->OnCurrentReserveAmmoChanged.AddDynamic(this, &UAmmoWidget::OnReserveAmmoChanged);
	BoundEquipment->OnCurrentClipAmmoChanged.AddDynamic(this, &UAmmoWidget::OnCurrentAmmoChanged);

	OnCurrentAmmoChanged(BoundEquipment->GetCurrentClipAmmo());
	OnReserveAmmoChanged(BoundEquipment->GetCurrentReserveAmmo());
}

void UAmmoWidget::Unbind()
{
	if (BoundEquipment)
	{
		BoundEquipment->OnCurrentReserveAmmoChanged.RemoveDynamic(this, &UAmmoWidget::OnReserveAmmoChanged);
		BoundEquipment->OnCurrentClipAmmoChanged.RemoveDynamic(this, &UAmmoWidget::OnCurrentAmmoChanged);
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

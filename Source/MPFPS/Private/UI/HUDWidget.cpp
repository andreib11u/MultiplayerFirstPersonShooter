// Copyright Andrei Bondarenko 2023


#include "UI/HUDWidget.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "UI/HUD/AmmoWidget.h"
#include "UI/HUD/HealthBarWidget.h"
#include "Weapons/EquipmentComponent.h"
#include "Weapons/Weapon.h"

void UHUDWidget::Init(UFPSAbilitySystemComponent* AbilitySystemComponent)
{
	HealthBarWidget->Init(AbilitySystemComponent);

	if (auto EquipmentComponent = AbilitySystemComponent->GetAvatarActor()->FindComponentByClass<UEquipmentComponent>())
	{
		AmmoWidget->Bind(EquipmentComponent);
		OnItemChanged(EquipmentComponent->GetCurrentItem());

		EquipmentComponent->OnCurrentItemChanged.AddDynamic(this, &UHUDWidget::OnItemChanged);
	}
}

void UHUDWidget::OnItemChanged(UEquippableItem* CurrentItem)
{
	if (CurrentItem && CurrentItem->IsA<UWeapon>())
	{
		AmmoWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		AmmoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

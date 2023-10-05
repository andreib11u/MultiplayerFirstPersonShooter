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
		if (auto Weapon = Cast<UWeapon>(EquipmentComponent->GetCurrentItem()))
		{
			AmmoWidget->Bind(Weapon);
			AmmoWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		EquipmentComponent->OnCurrentItemChanged.AddDynamic(this, &UHUDWidget::OnItemChanged);
	}
	
}

void UHUDWidget::OnItemChanged(UEquippableItem* CurrentItem)
{
	if (auto Weapon = Cast<UWeapon>(CurrentItem))
	{
		AmmoWidget->Bind(Weapon);
		AmmoWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		AmmoWidget->Unbind();
		AmmoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

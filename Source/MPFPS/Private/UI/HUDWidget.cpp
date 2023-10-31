// Copyright Andrei Bondarenko 2023

#include "UI/HUDWidget.h"
#include "Characters/PlayerCharacter.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "UI/HUD/AmmoWidget.h"
#include "UI/HUD/DamageDirectionIndicatorWidget.h"
#include "UI/HUD/HealthBarWidget.h"
#include "Weapons/EquipmentComponent.h"
#include "Weapons/Weapon.h"

void UHUDWidget::Init(UFPSAbilitySystemComponent* AbilitySystemComponent)
{
	HealthBarWidget->Init(AbilitySystemComponent);

	CharacterOwner = Cast<APlayerCharacter>(AbilitySystemComponent->GetAvatarActor());
	check(CharacterOwner);

	if (auto EquipmentComponent = AbilitySystemComponent->GetAvatarActor()->FindComponentByClass<UEquipmentComponent>())
	{
		AmmoWidget->Bind(EquipmentComponent);
		OnItemChanged(EquipmentComponent->GetCurrentItem());

		EquipmentComponent->OnCurrentItemChanged.AddDynamic(this, &UHUDWidget::OnItemChanged);
	}
}

void UHUDWidget::AddDamageDirectionIndicator(AActor* DamageCauser)
{
	UDamageDirectionIndicatorWidget** FoundDamageIndicator = DamageDirectionIndicators.Find(DamageCauser);
	if (!FoundDamageIndicator)
	{
		auto DamageDirectionIndicator = CreateWidget<UDamageDirectionIndicatorWidget>(this, *DamageDirectionIndicatorClass);
		DamageDirectionIndicator->Init(CharacterOwner, DamageCauser);
		DamageDirectionIndicators.Add(DamageCauser, DamageDirectionIndicator);

		UCanvasPanelSlot* CanvasSlot = HUDCanvas->AddChildToCanvas(DamageDirectionIndicator);
		CanvasSlot->SetAnchors(FAnchors{ 0.5f, 0.5f, 0.5f, 0.5f });
		CanvasSlot->SetPosition(FVector2D::ZeroVector);
		CanvasSlot->SetSize(FVector2D{ 100.f, 60.f });
		CanvasSlot->SetAlignment(FVector2D{ 0.5f, 0.5f });
	}
	else
	{
		if (*FoundDamageIndicator)
		{
			(*FoundDamageIndicator)->ResetTimer();
		}
	}
}

void UHUDWidget::DisplayDamageDirectionIndicators()
{
	TArray<TPair<AActor*, UDamageDirectionIndicatorWidget*>> DamageDirectionWidgets = DamageDirectionIndicators.Array();

	for (auto DamageDirectionWidget : DamageDirectionWidgets)
	{
		if (DamageDirectionWidget.Key)
		{
			if (DamageDirectionWidget.Value->IsActive())
			{
				DamageDirectionWidget.Value->CalculateTransform();
			}
			else
			{
				DamageDirectionIndicators.Remove(DamageDirectionWidget.Key);
			}
		}
		else
		{
			DamageDirectionIndicators.Remove(DamageDirectionWidget.Key);
		}
	}
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	DisplayDamageDirectionIndicators();
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

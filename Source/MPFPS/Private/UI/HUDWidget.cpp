// Copyright Andrei Bondarenko 2023


#include "UI/HUDWidget.h"
#include "UI/HUD/HealthBarWidget.h"

void UHUDWidget::Init(UFPSAbilitySystemComponent* AbilitySystemComponent)
{
	HealthBarWidget->Init(AbilitySystemComponent);
}

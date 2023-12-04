// Copyright Andrei Bondarenko 2023


#include "UI/MainMenu/Settings/UserSettingsWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UUserSettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GeneralSettingsButton->OnClicked.AddDynamic(this, &UUserSettingsWidget::OnGeneralSettingsButtonClicked);
	AudioSettingsButton->OnClicked.AddDynamic(this, &UUserSettingsWidget::OnAudioSettingsButtonClicked);
	GraphicsSettingsButton->OnClicked.AddDynamic(this, &UUserSettingsWidget::OnGraphicsSettingsButtonClicked);
}

void UUserSettingsWidget::OnGeneralSettingsButtonClicked()
{
	SettingsWidgetSwitcher->SetActiveWidgetIndex(0);
}

void UUserSettingsWidget::OnAudioSettingsButtonClicked()
{
	SettingsWidgetSwitcher->SetActiveWidgetIndex(1);
}

void UUserSettingsWidget::OnGraphicsSettingsButtonClicked()
{
	SettingsWidgetSwitcher->SetActiveWidgetIndex(2);
}

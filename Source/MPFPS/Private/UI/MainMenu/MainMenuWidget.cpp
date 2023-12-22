// Copyright Andrei Bondarenko 2023


#include "UI/MainMenu/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subsystems/SessionsSubsystem.h"
#include "UI/MainMenu/CreateSessionWidget.h"
#include "UI/MainMenu/FindSessionsWidget.h"
#include "UI/MainMenu/Settings/UserSettingsWidget.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitButtonClicked);
	CreateSessionButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCreateSessionButtonClicked);
	FindSessionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnFindSessionsButtonClicked);
	SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingsButtonClicked);
}

void UMainMenuWidget::OnCreateSessionButtonClicked()
{
	auto CreateSessionWidget = CreateWidget<UCreateSessionWidget>(GetOwningPlayer(), *CreateSessionWidgetClass);
	if (CreateSessionWidget)
	{
		CreateSessionWidget->AddToViewport();
		CreateSessionWidget->SetParentMenu(this);

		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainMenuWidget::OnFindSessionsButtonClicked()
{
	auto FindSessionsWidget = CreateWidget<UFindSessionsWidget>(GetOwningPlayer(), *FindSessionsWidgetClass);
	if (FindSessionsWidget)
	{
		FindSessionsWidget->AddToViewport();
		FindSessionsWidget->SetParentMenu(this);

		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainMenuWidget::OnQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UMainMenuWidget::OnSettingsButtonClicked()
{
	auto SettingsWidget = CreateWidget<UUserSettingsWidget>(GetOwningPlayer(), *UserSettingsWidgetClass);
	if (SettingsWidget)
	{
		SettingsWidget->AddToViewport();
		SettingsWidget->SetParentMenu(this);

		SetVisibility(ESlateVisibility::Collapsed);
	}
}

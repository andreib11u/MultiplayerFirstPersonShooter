// Copyright Andrei Bondarenko 2023


#include "GameplayFramework/MainMenuPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/MainMenu/MainMenuWidget.h"

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());

	auto MainMenuWidget = CreateWidget<UMainMenuWidget>(this, *MainMenuWidgetClass);
	if (MainMenuWidget)
	{
		MainMenuWidget->AddToViewport();
	}
}

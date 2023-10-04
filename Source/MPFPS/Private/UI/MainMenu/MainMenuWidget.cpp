// Copyright Andrei Bondarenko 2023


#include "UI/MainMenu/MainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subsystems/SessionsSubsystem.h"
#include "UI/MainMenu/FindSessionsWidget.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitButtonClicked);
	CreateSessionButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCreateSessionButtonClicked);
	FindSessionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnFindSessionsButtonClicked);
}

void UMainMenuWidget::OnSessionStarted(bool bSuccessful)
{
	if (bSuccessful)
	{
		// /Game/MPFPS/Maps/Map_Blank
		// /Game/FirstPerson/Maps/FirstPersonMap
		UGameplayStatics::OpenLevel(GetWorld(), "/Game/FirstPerson/Maps/FirstPersonMap", true, "listen"); // todo: make it a reference to a level
	}
	else
	{
		if (GEngine)
		{
			const FString OnScreenMessage = FString::Printf(TEXT("Couldn't successfully start a session"));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, OnScreenMessage);
		}

		CreateSessionButton->SetIsEnabled(true);
	}
}

void UMainMenuWidget::OnSessionCreated(bool bSuccessful)
{
	if (bSuccessful)
	{
		USessionsSubsystem* SessionsSubsystem = GetSessionsSubsystem();
		if (SessionsSubsystem)
		{
			/*SessionsSubsystem->OnStartSessionCompleteEvent.AddDynamic(this, &UMainMenuWidget::OnSessionStarted);
			SessionsSubsystem->StartSession();*/
			OnSessionStarted(true);
		}
	}
	else
	{
		if (GEngine)
		{
			const FString OnScreenMessage = FString::Printf(TEXT("Couldn't successfully create a session"));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, OnScreenMessage);
		}

		CreateSessionButton->SetIsEnabled(true);
	}
}

USessionsSubsystem* UMainMenuWidget::GetSessionsSubsystem() const
{
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return nullptr;
	}

	auto SessionsSubsystem = GameInstance->GetSubsystem<USessionsSubsystem>();
	if (!SessionsSubsystem)
	{
		return nullptr;
	}

	return SessionsSubsystem;
}

void UMainMenuWidget::OnCreateSessionButtonClicked()
{
	USessionsSubsystem* SessionsSubsystem = GetSessionsSubsystem();
	if (!SessionsSubsystem)
	{
		return;
	}

	SessionsSubsystem->OnCreateSessionCompleteEvent.AddDynamic(this, &UMainMenuWidget::OnSessionCreated);
	SessionsSubsystem->CreateSession(8, true);

	CreateSessionButton->SetIsEnabled(false);
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

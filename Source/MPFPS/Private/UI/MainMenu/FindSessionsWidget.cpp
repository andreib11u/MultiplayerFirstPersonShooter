// Copyright Andrei Bondarenko 2023

#include "UI/MainMenu/FindSessionsWidget.h"

#include "OnlineSessionSettings.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CircularThrobber.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Subsystems/SessionsSubsystem.h"
#include "UI/MainMenu/FoundSessionEntryWidget.h"
#include "UI/MainMenu/MainMenuWidget.h"

void UFindSessionsWidget::SetParentMenu(UMainMenuWidget* InParentMenu)
{
	ParentMenu = InParentMenu;
}

void UFindSessionsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SearchSessionsButton->OnClicked.AddDynamic(this, &UFindSessionsWidget::OnSearchSessionsButtonClicked);
	BackButton->OnClicked.AddDynamic(this, &UFindSessionsWidget::OnBackButtonClicked);
}

void UFindSessionsWidget::OnSearchSessionsButtonClicked()
{
	USessionsSubsystem* SessionsSubsystem = GetSessionsSubsystem();
	if (SessionsSubsystem)
	{
		FindSessionsCompleteDelegateHandle = SessionsSubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &UFindSessionsWidget::OnSessionsFound);
		SessionsSubsystem->FindSessions(10, true);

		SearchSessionsButton->SetIsEnabled(false);
		Throbber->SetVisibility(ESlateVisibility::Visible);

		FoundSessionsPanel->ClearChildren();
	}
}

void UFindSessionsWidget::OnSessionsFound(const TArray<FOnlineSessionSearchResult>& OnlineSessionSearchResults, bool bArg)
{
	SearchSessionsButton->SetIsEnabled(true);
	Throbber->SetVisibility(ESlateVisibility::Collapsed);

	USessionsSubsystem* SessionsSubsystem = GetSessionsSubsystem();
	if (SessionsSubsystem)
	{
		 SessionsSubsystem->OnFindSessionsCompleteEvent.Remove(FindSessionsCompleteDelegateHandle);
	}

	if (OnlineSessionSearchResults.IsEmpty())
	{
		auto NoResultsText = WidgetTree->ConstructWidget<UTextBlock>();
		NoResultsText->SetText(NSLOCTEXT("UI", "NoSessionsFound", "No Sessions Found"));
		NoResultsText->SetJustification(ETextJustify::Center);

		UVerticalBoxSlot* VerticalBoxSlot = FoundSessionsPanel->AddChildToVerticalBox(NoResultsText);
		VerticalBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

		return;
	}

	for (const FOnlineSessionSearchResult& SearchResult : OnlineSessionSearchResults)
	{
		auto FoundSessionWidget = CreateWidget<UFoundSessionEntryWidget>(GetOwningPlayer(), *FoundSessionEntryWidgetClass);
		if (FoundSessionWidget)
		{
			FoundSessionWidget->Init(SearchResult);
			FoundSessionsPanel->AddChildToVerticalBox(FoundSessionWidget);
		}
	}
}

void UFindSessionsWidget::OnBackButtonClicked()
{
	ParentMenu->SetVisibility(ESlateVisibility::Visible);

	USessionsSubsystem* SessionsSubsystem = GetSessionsSubsystem();
	if (SessionsSubsystem && FindSessionsCompleteDelegateHandle.IsValid())
	{
		SessionsSubsystem->OnFindSessionsCompleteEvent.Remove(FindSessionsCompleteDelegateHandle);
	}

	RemoveFromParent();
}

USessionsSubsystem* UFindSessionsWidget::GetSessionsSubsystem() const
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

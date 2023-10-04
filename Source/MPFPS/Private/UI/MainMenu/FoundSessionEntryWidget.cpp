// Copyright Andrei Bondarenko 2023

#include "UI/MainMenu/FoundSessionEntryWidget.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Subsystems/SessionsSubsystem.h"

void UFoundSessionEntryWidget::Init(const FOnlineSessionSearchResult& SessionSearchResult)
{
	const FText SessionNameText = FText::FromString(SessionSearchResult.GetSessionIdStr());
	SessionName->SetText(SessionNameText);

	JoinButton->OnClicked.AddDynamic(this, &UFoundSessionEntryWidget::OnJoinButtonClicked);

	FoundSession = SessionSearchResult;
}

void UFoundSessionEntryWidget::JoinSessionComplete(EOnJoinSessionCompleteResult::Type Arg)
{
	JoinButton->SetIsEnabled(true);

	if (Arg != EOnJoinSessionCompleteResult::Success)
	{
		FString Error;
		switch (Arg)
		{
		case EOnJoinSessionCompleteResult::Success: Error = "Success";break;
		case EOnJoinSessionCompleteResult::SessionIsFull: Error = "SessionIsFull"; break;
		case EOnJoinSessionCompleteResult::SessionDoesNotExist: Error = "SessionDoesNotExist"; break;
		case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress: Error = "CouldNotRetrieveAddress"; break;
		case EOnJoinSessionCompleteResult::AlreadyInSession: Error = "AlreadyInSession"; break;
		case EOnJoinSessionCompleteResult::UnknownError: Error = "UnknownError"; break;
		default: checkNoEntry();
		}

		if (GEngine)
		{
			const FString OnScreenMessage = FString::Printf(TEXT("Can't join the session. Reason: %s"), *Error);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, OnScreenMessage);
		}
	}
	else
	{
		bool bTravelSuccess = GetSessionsSubsystem()->TryTravelToCurrentSession();
		UE_LOG(LogTemp, Warning, TEXT("bTravelSuccess: %i"), bTravelSuccess)
	}

	if (GEngine)
	{
		const FString OnScreenMessage = FString::Printf(TEXT("Join Session Complete"));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, OnScreenMessage);
	}

	USessionsSubsystem* SessionSubsystem = GetSessionsSubsystem();
	if (SessionSubsystem)
	{
		SessionSubsystem->OnJoinGameSessionCompleteEvent.Remove(JoinSessionCompleteDelegateHandle);
	}
}

void UFoundSessionEntryWidget::OnJoinButtonClicked()
{
	USessionsSubsystem* SessionSubsystem = GetSessionsSubsystem();
	if (SessionSubsystem)
	{
		JoinSessionCompleteDelegateHandle =
			SessionSubsystem->OnJoinGameSessionCompleteEvent.AddUObject(this, &UFoundSessionEntryWidget::JoinSessionComplete);
		SessionSubsystem->JoinGameSession(FoundSession);

		JoinButton->SetIsEnabled(false);
	}
}

USessionsSubsystem* UFoundSessionEntryWidget::GetSessionsSubsystem() const
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

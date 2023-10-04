// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/SessionsSubsystem.h"
#include "UI/FPSUserWidget.h"
#include "FoundSessionEntryWidget.generated.h"

class USessionsSubsystem;
class UButton;
class UTextBlock;
/**
 *
 */
UCLASS()
class MPFPS_API UFoundSessionEntryWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void Init(const FOnlineSessionSearchResult& SessionSearchResult);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SessionName;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	FOnlineSessionSearchResult FoundSession;
	FDelegateHandle JoinSessionCompleteDelegateHandle;

	void JoinSessionComplete(EOnJoinSessionCompleteResult::Type Arg);
	UFUNCTION()
	void OnJoinButtonClicked();

	USessionsSubsystem* GetSessionsSubsystem() const;
};

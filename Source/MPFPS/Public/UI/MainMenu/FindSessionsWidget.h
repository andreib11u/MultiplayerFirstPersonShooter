// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UI/FPSUserWidget.h"
#include "FindSessionsWidget.generated.h"

class UVerticalBox;
class UFoundSessionEntryWidget;
class USessionsSubsystem;
class UMainMenuWidget;
class UCircularThrobber;
class UButton;
/**
 * 
 */
UCLASS()
class MPFPS_API UFindSessionsWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void SetParentMenu(UMainMenuWidget* InParentMenu);
protected:
	virtual void NativeOnInitialized() override;
private:
	UPROPERTY(meta = (BindWidget))
	UButton* SearchSessionsButton;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* FoundSessionsPanel;
	UPROPERTY(meta = (BindWidget))
	UCircularThrobber* Throbber;
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY()
	UMainMenuWidget* ParentMenu;

	FDelegateHandle FindSessionsCompleteDelegateHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFoundSessionEntryWidget> FoundSessionEntryWidgetClass;

	UFUNCTION()
	void OnSearchSessionsButtonClicked();
	void OnSessionsFound(const TArray<FOnlineSessionSearchResult>& OnlineSessionSearchResults, bool bArg);
	UFUNCTION()
	void OnBackButtonClicked();

	USessionsSubsystem* GetSessionsSubsystem() const;
};

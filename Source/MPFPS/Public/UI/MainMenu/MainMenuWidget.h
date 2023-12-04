// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UI/FPSUserWidget.h"
#include "MainMenuWidget.generated.h"

class UUserSettingsWidget;
class UFindSessionsWidget;
class USessionsSubsystem;
class UButton;
/**
 *
 */
UCLASS()
class MPFPS_API UMainMenuWidget : public UFPSUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
private:
	UPROPERTY(meta = (BindWidget))
	UButton* CreateSessionButton;
	UPROPERTY(meta = (BindWidget))
	UButton* FindSessionsButton;
	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFindSessionsWidget> FindSessionsWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserSettingsWidget> UserSettingsWidgetClass;

	UFUNCTION()
	void OnSessionStarted(bool bSuccessful);
	UFUNCTION()
	void OnSessionCreated(bool bSuccessful);
	UFUNCTION()
	void OnCreateSessionButtonClicked();
	UFUNCTION()
	void OnFindSessionsButtonClicked();
	UFUNCTION()
	void OnQuitButtonClicked();
	UFUNCTION()
	void OnSettingsButtonClicked();

	USessionsSubsystem* GetSessionsSubsystem() const;
};

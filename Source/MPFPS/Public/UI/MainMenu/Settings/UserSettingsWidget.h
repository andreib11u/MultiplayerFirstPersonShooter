// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UI/FPSUserWidget.h"
#include "UserSettingsWidget.generated.h"

class UMainMenuWidget;
class UButton;
class UWidgetSwitcher;
/**
 *
 */
UCLASS()
class MPFPS_API UUserSettingsWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void SetParentMenu(UMainMenuWidget* MainMenuWidget) { ParentMenu = MainMenuWidget; }

protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* GeneralSettingsButton;
	UPROPERTY(meta = (BindWidget))
	UButton* AudioSettingsButton;
	UPROPERTY(meta = (BindWidget))
	UButton* GraphicsSettingsButton;
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* SettingsWidgetSwitcher;

	UFUNCTION()
	void OnGeneralSettingsButtonClicked();
	UFUNCTION()
	void OnAudioSettingsButtonClicked();
	UFUNCTION()
	void OnGraphicsSettingsButtonClicked();

	UPROPERTY()
	UMainMenuWidget* ParentMenu;
};

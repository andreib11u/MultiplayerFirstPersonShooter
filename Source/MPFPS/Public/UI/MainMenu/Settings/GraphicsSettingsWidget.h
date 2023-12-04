// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UI/FPSUserWidget.h"
#include "GraphicsSettingsWidget.generated.h"

class UFPSGameUserSettings;
class UOptionChooserWidget;
/**
 *
 */
UCLASS()
class MPFPS_API UGraphicsSettingsWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void UpdateSettings();
	void Init();

protected:
	virtual void NativeOnInitialized() override;
	void ApplySettings();

private:
	UPROPERTY(meta = (BindWidget))
	UOptionChooserWidget* FullscreenModeOption;
	UPROPERTY(meta = (BindWidget))
	UOptionChooserWidget* ScreenResolutionOption;
	UPROPERTY(meta = (BindWidget))
	UOptionChooserWidget* FramerateLimitationOption;
	UPROPERTY(meta = (BindWidget))
	UOptionChooserWidget* VerticalSyncOption;
	UPROPERTY(meta = (BindWidget))
	UOptionChooserWidget* OverallQualityOption;
	UPROPERTY(meta = (BindWidget))
	UOptionChooserWidget* TextureQualityOption;
	UPROPERTY(meta = (BindWidget))
	UOptionChooserWidget* ShadowQualityOption;
	UPROPERTY(meta = (BindWidget))
	UOptionChooserWidget* VisualEffectsQuality;
	UPROPERTY(meta = (BindWidget))
	UOptionChooserWidget* AntiAliasingOption;

	UFUNCTION()
	void OnFullscreenModeChanged(int32 OptionIndex);
	UFUNCTION()
	void OnFramerateLimitationChanged(int32 OptionIndex);
	UFUNCTION()
	void OnVerticalSyncChanged(int32 OptionIndex);
	UFUNCTION()
	void OnOverallQualityChanged(int32 OptionIndex);
	UFUNCTION()
	void OnTextureQualityChanged(int32 OptionIndex);
	UFUNCTION()
	void OnShadowQualityChanged(int32 OptionIndex);
	UFUNCTION()
	void OnVisualEffectsQualityChanged(int32 OptionIndex);
	UFUNCTION()
	void OnAntiAliasingChanged(int32 OptionIndex);

	TWeakObjectPtr<UFPSGameUserSettings> Settings;
};

// Copyright Andrei Bondarenko 2023


#include "UI/MainMenu/Settings/GraphicsSettingsWidget.h"
#include "MPFPS/MPFPS.h"
#include "Settings/FPSGameUserSettings.h"
#include "UI/OptionChooserWidget.h"

void UGraphicsSettingsWidget::UpdateSettings()
{
	FullscreenModeOption->SetOptionIndex(Settings->GetFullscreenMode(), false);
	FramerateLimitationOption->SetOptionIndex(FMath::RoundToInt32(Settings->GetFrameRateLimit() / 30.f), false);
	VerticalSyncOption->SetOptionIndex(Settings->IsVSyncEnabled(), false);
	OverallQualityOption->SetOptionIndex(Settings->GetOverallScalabilityLevel(), false);
	TextureQualityOption->SetOptionIndex(Settings->GetTextureQuality(), false);
	ShadowQualityOption->SetOptionIndex(Settings->GetShadowQuality(), false);
	VisualEffectsQuality->SetOptionIndex(Settings->GetVisualEffectQuality(), false);
	AntiAliasingOption->SetOptionIndex(Settings->GetAntiAliasingQuality(), false);
}

void UGraphicsSettingsWidget::Init()
{
	Settings = UFPSGameUserSettings::GetFPSGameUserSettings();

	if (!Settings.IsValid())
	{
		UE_LOG(LogUI, Error, TEXT("Settings ptr is null in %s"), *GetName())
		return;
	}

	UpdateSettings();

	FullscreenModeOption->OnOptionIndexChanged.AddDynamic(this, &UGraphicsSettingsWidget::OnFullscreenModeChanged);
	FramerateLimitationOption->OnOptionIndexChanged.AddDynamic(this, &UGraphicsSettingsWidget::OnFramerateLimitationChanged);
	VerticalSyncOption->OnOptionIndexChanged.AddDynamic(this, &UGraphicsSettingsWidget::OnVerticalSyncChanged);
	OverallQualityOption->OnOptionIndexChanged.AddDynamic(this, &UGraphicsSettingsWidget::OnOverallQualityChanged);
	TextureQualityOption->OnOptionIndexChanged.AddDynamic(this, &UGraphicsSettingsWidget::OnTextureQualityChanged);
	ShadowQualityOption->OnOptionIndexChanged.AddDynamic(this, &UGraphicsSettingsWidget::OnShadowQualityChanged);
	VisualEffectsQuality->OnOptionIndexChanged.AddDynamic(this, &UGraphicsSettingsWidget::OnVisualEffectsQualityChanged);
	AntiAliasingOption->OnOptionIndexChanged.AddDynamic(this, &UGraphicsSettingsWidget::OnAntiAliasingChanged);
}

void UGraphicsSettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Init();
}

void UGraphicsSettingsWidget::ApplySettings()
{
	Settings->SaveSettings();
	Settings->ApplyNonResolutionSettings();
}

void UGraphicsSettingsWidget::OnFullscreenModeChanged(int32 OptionIndex)
{
	Settings->SetFullscreenMode(static_cast<EWindowMode::Type>(OptionIndex));
	ApplySettings();
}

void UGraphicsSettingsWidget::OnFramerateLimitationChanged(int32 OptionIndex)
{
	Settings->SetFrameRateLimit(OptionIndex * 30.f);
	ApplySettings();
	UpdateSettings();
}

void UGraphicsSettingsWidget::OnVerticalSyncChanged(int32 OptionIndex)
{
	Settings->SetVSyncEnabled(OptionIndex != 0);
	ApplySettings();
	UpdateSettings();
}

void UGraphicsSettingsWidget::OnOverallQualityChanged(int32 OptionIndex)
{
	Settings->SetOverallScalabilityLevel(OptionIndex);
	ApplySettings();
	UpdateSettings();
}

void UGraphicsSettingsWidget::OnTextureQualityChanged(int32 OptionIndex)
{
	Settings->SetTextureQuality(OptionIndex);
	ApplySettings();
	UpdateSettings();
}

void UGraphicsSettingsWidget::OnShadowQualityChanged(int32 OptionIndex)
{
	Settings->SetShadowQuality(OptionIndex);
	ApplySettings();
	UpdateSettings();
}

void UGraphicsSettingsWidget::OnVisualEffectsQualityChanged(int32 OptionIndex)
{
	Settings->SetVisualEffectQuality(OptionIndex);
	ApplySettings();
	UpdateSettings();
}

void UGraphicsSettingsWidget::OnAntiAliasingChanged(int32 OptionIndex)
{
	Settings->SetAntiAliasingQuality(OptionIndex);
	ApplySettings();
	UpdateSettings();
}

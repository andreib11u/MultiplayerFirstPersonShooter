// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UI/FPSUserWidget.h"
#include "GeneralSettingsWidget.generated.h"

class UTextBlock;
class USlider;
/**
 * 
 */
UCLASS()
class MPFPS_API UGeneralSettingsWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:

private:
	UPROPERTY(meta = (BindWidget))
	USlider* MouseSensitivitySlider;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MouseSensitivityText;

};

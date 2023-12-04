// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UI/FPSUserWidget.h"
#include "AudioSettingsWidget.generated.h"

class USlider;
/**
 * 
 */
UCLASS()
class MPFPS_API UAudioSettingsWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:

private:
	UPROPERTY(meta = (BindWidget))
	USlider* AudioSlider;
};

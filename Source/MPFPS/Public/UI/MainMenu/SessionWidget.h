// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UI/FPSUserWidget.h"
#include "SessionWidget.generated.h"

struct FMapOptions;
class UOptionChooserWidget;
class UTextBlock;

/**
 * 
 */
UCLASS()
class MPFPS_API USessionWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void Display(const FMapOptions& Map);

	UOptionChooserWidget* GetMapModeChooser() const { return MapModeChooser; }
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MapNameText;
	UPROPERTY(meta = (BindWidget))
	UOptionChooserWidget* MapModeChooser;
};

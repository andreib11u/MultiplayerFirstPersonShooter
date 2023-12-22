// Copyright Andrei Bondarenko 2023


#include "UI/MainMenu/SessionWidget.h"
#include "Components/TextBlock.h"
#include "MPFPS/MPFPS.h"
#include "Types/FPSTypes.h"
#include "UI/OptionChooserWidget.h"

void USessionWidget::Display(const FMapOptions& Map)
{
	MapNameText->SetText(Map.MapName);

	if (Map.AvailableModes.IsEmpty())
	{
		UE_LOG(LogUI, Error, TEXT("Map %s doesn't have any available mode"), *Map.Map.ToString())
		return;
	}
	
	TArray<FText> ModeOptions;
	auto ModeNames = FMapOptions::GetLocalizedModeNames();
	for (FName Mode : Map.AvailableModes)
	{
		FText* FoundName = ModeNames.Find(Mode);
		if (FoundName)
		{
			ModeOptions.Add(*FoundName);
		}
		else
		{
			UE_LOG(LogUI, Warning, TEXT("There is no map mode with a name %s"), *Mode.ToString())
		}
	}

	MapModeChooser->SetOptions(ModeOptions);
	MapModeChooser->SetOptionIndex(0, false);
}

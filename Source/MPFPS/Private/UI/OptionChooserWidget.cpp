// Copyright Andrei Bondarenko 2023


#include "UI/OptionChooserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UOptionChooserWidget::SetOptionIndex(int32 Index, bool bBroadcast)
{
	CurrentIndex = Index;

	if (Index == INDEX_NONE)
	{
		OptionText->SetText(NSLOCTEXT("UI", "CustomSetting", "Custom"));
		return;
	}

	OptionText->SetText(Options[CurrentIndex]);

	if (bBroadcast)
	{
		OnOptionIndexChanged.Broadcast(CurrentIndex);
	}
}

void UOptionChooserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	LeftButton->OnClicked.AddDynamic(this, &UOptionChooserWidget::OnLeftButtonClicked);
	RightButton->OnClicked.AddDynamic(this, &UOptionChooserWidget::OnRightButtonClicked);
}

void UOptionChooserWidget::OnLeftButtonClicked()
{
	if (!GetOptions().IsEmpty())
	{
		if (--CurrentIndex < 0)
		{
			CurrentIndex = GetOptions().Num() - 1;
		}

		SetOptionIndex(CurrentIndex);
	}
}

void UOptionChooserWidget::OnRightButtonClicked()
{
	if (!GetOptions().IsEmpty())
	{
		if (++CurrentIndex >= GetOptions().Num())
		{
			CurrentIndex = 0;
		}

		SetOptionIndex(CurrentIndex);
	}
}

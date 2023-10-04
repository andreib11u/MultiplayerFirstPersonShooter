// Copyright Andrei Bondarenko 2023


#include "UI/FPSHUD.h"
#include "Blueprint/UserWidget.h"
#include "Types/UITypes.h"
#include "UI/HUDWidget.h"

void AFPSHUD::CreateHUD()
{
	HUDWidget = CreateWidget<UHUDWidget>(GetOwningPlayerController(), *HUDWidgetClass);
	check(HUDWidget)

	HUDWidget->AddToViewport(HUD);
}

void AFPSHUD::DestroyHUD()
{
	if (HUDWidget)
	{
		HUDWidget->RemoveFromParent();
	}
}

void AFPSHUD::BeginPlay()
{
	Super::BeginPlay();
}

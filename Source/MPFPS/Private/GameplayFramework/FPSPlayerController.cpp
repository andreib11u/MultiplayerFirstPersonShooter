// Copyright Andrei Bondarenko 2023

#include "GameplayFramework/FPSPlayerController.h"

#include "GameplayFramework/FPSPlayerState.h"
#include "UI/FPSHUD.h"
#include "UI/HUDWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPSPlayerController, All, All);

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == NM_ListenServer)
	{
		InitializeHUD();
	}
}

void AFPSPlayerController::InitializeHUD()
{
	if (auto HUD = Cast<AFPSHUD>(GetHUD()))
	{
		HUD->AddHUDToScreen();
		UHUDWidget* HUDWidget = HUD->GetHUDWidget();
		auto FPSPlayerState = GetPlayerState<AFPSPlayerState>();
		check(FPSPlayerState);

		UFPSAbilitySystemComponent* AbilitySystemComponent = FPSPlayerState->GetFPSAbilitySystemComponent();
		check(AbilitySystemComponent);

		HUDWidget->Init(AbilitySystemComponent);
	}
	else
	{
		UE_LOG(LogFPSPlayerController, Error, TEXT("HUD class should be FPSHUD"))
	}
}

void AFPSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeHUD();
}

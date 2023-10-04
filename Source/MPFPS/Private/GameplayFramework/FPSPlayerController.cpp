// Copyright Andrei Bondarenko 2023

#include "GameplayFramework/FPSPlayerController.h"
#include "GameplayFramework/FPSPlayerState.h"
#include "UI/FPSHUD.h"
#include "UI/HUDWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPSPlayerController, All, All);

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());

	if (GetNetMode() == NM_ListenServer)
	{
		InitializeHUD();
	}
}

void AFPSPlayerController::InitializeHUD()
{
	if (IsLocalPlayerController())
	{
		if (auto* HUD = Cast<AFPSHUD>(GetHUD()))
		{
			HUD->CreateHUD();
			UHUDWidget* HUDWidget = HUD->GetHUDWidget();
			auto* FPSPlayerState = GetPlayerState<AFPSPlayerState>();
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
}

void AFPSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeHUD();
}

void AFPSPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

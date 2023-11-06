// Copyright Andrei Bondarenko 2023

#include "GameplayFramework/FPSGameState.h"
#include "Net/UnrealNetwork.h"
#include "Settings/FPSSessionSettings.h"

void AFPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSGameState, SessionSettingsClass);
}
void AFPSGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (SessionSettingsClass)
	{
		SessionSettings = NewObject<UFPSSessionSettings>(this, SessionSettingsClass);
	}
}

void AFPSGameState::OnRep_SessionSettingsClass()
{
	if (SessionSettingsClass)
	{
		SessionSettings = NewObject<UFPSSessionSettings>(this, SessionSettingsClass);
	}
}

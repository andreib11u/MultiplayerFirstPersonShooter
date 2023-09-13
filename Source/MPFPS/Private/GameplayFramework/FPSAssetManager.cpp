// Copyright Andrei Bondarenko 2023


#include "GameplayFramework/FPSAssetManager.h"
#include "AbilitySystemGlobals.h"

void UFPSAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}

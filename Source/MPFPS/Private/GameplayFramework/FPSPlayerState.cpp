// Copyright Andrei Bondarenko 2023


#include "GameplayFramework/FPSPlayerState.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"

AFPSPlayerState::AFPSPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UFPSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

UAbilitySystemComponent* AFPSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Copyright Andrei Bondarenko 2023

#include "GameplayFramework/FPSPlayerState.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "GameplayAbilitySystem/AttributeSets/PlayerAttributeSet.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPSPlayerState, All, All);

AFPSPlayerState::AFPSPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UFPSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AFPSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFPSPlayerState::BeginPlay()
{
	Super::BeginPlay();
}


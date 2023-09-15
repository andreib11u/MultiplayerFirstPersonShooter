// Copyright Andrei Bondarenko 2023

#include "GameplayFramework/FPSPlayerState.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "GameplayAbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "UI/FPSHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPSPlayerState, All, All);

AFPSPlayerState::AFPSPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UFPSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));
	AttributeSet->InitCurrentHealth(100.f);
	AttributeSet->InitMaxHealth(100.f);
	AttributeSet->InitWalkSpeed(600.f);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentHealthAttribute())
		.AddUObject(this, &AFPSPlayerState::CurrentHealthChanged);
}

UAbilitySystemComponent* AFPSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFPSPlayerState::BeginPlay()
{
	Super::BeginPlay();

}

void AFPSPlayerState::CurrentHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (OnAttributeChangeData.NewValue == 0.f)
	{
		// todo: ded
	}
}

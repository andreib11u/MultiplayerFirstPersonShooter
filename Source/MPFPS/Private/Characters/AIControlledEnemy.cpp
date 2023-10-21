// Copyright Andrei Bondarenko 2023


#include "Characters/AIControlledEnemy.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"

AAIControlledEnemy::AAIControlledEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UFPSAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

UAbilitySystemComponent* AAIControlledEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAIControlledEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
	}
}


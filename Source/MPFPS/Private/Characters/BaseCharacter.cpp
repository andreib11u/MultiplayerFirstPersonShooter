// Copyright Andrei Bondarenko 2023


#include "Characters/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "GameplayAbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"
#include "Types/CollisionTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(BULLET_TRACE_COLLISION, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(BULLET_TRACE_COLLISION, ECR_Overlap);
}

void ABaseCharacter::InitializeAttributes()
{
	if (!AttributeInitializationEffect)
	{
		UE_LOG(LogBaseCharacter, Error, TEXT("%s missing AttributeInitializationEffect. Attributes are not initialized"), *GetName())
		return;
	}

	if (auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(this))
	{
		auto AbilitySystemComponent = Cast<UFPSAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent());
		if (!AbilitySystemComponent)
		{
			return;
		}
		
		const auto* AttributeSet = AbilitySystemComponent->GetAttributeSet<UBaseAttributeSet>();

		// set attribute callbacks
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetWalkSpeedAttribute())
			.AddUObject(this, &ABaseCharacter::OnWalkSpeedChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentHealthAttribute())
			.AddUObject(this, &ABaseCharacter::OnCurrentHealthChanged);

		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		// apply initialization effect
		const FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(AttributeInitializationEffect, 1.f, EffectContext);
		if (EffectSpec.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
		}
	}
}

void ABaseCharacter::GrantAbilities()
{
	if (auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(this))
	{
		auto AbilitySystemComponent = Cast<UFPSAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent());
		if (!AbilitySystemComponent)
		{
			return;
		}

		for (TSubclassOf<UFPSGameplayAbility> Ability : Abilities)
		{
			FGameplayAbilitySpec Spec = FGameplayAbilitySpec(Ability, 1, 0, this);

			if (ensure(AbilitySystemComponent))
			{
				AbilitySystemComponent->GiveAbility(Spec);
			}
		}
	}

	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::OnWalkSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	GetCharacterMovement()->MaxWalkSpeed = OnAttributeChangeData.NewValue;
}

void ABaseCharacter::OnCurrentHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (OnAttributeChangeData.NewValue <= 0.f)
	{
		Death();
	}
}

void ABaseCharacter::Death()
{
	if (GetController() && GetController()->IsA<AAIController>())
	{
		GetController()->UnPossess();
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

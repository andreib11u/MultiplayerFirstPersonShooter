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
#include "Kismet/KismetSystemLibrary.h"
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
			auto AbilityCDO = Cast<UFPSGameplayAbility>(Ability->GetDefaultObject());
			FGameplayAbilitySpec Spec = FGameplayAbilitySpec(Ability, 1, static_cast<int32>(AbilityCDO->GetAbilityInput()), this);

			if (ensure(AbilitySystemComponent))
			{
				AbilitySystemComponent->GiveAbility(Spec);
			}
		}
	}
}

void ABaseCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	Team = static_cast<EFPSTeam>(TeamID.GetId());
}

FGenericTeamId ABaseCharacter::GetGenericTeamId() const
{
	return static_cast<uint8>(Team);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float RightFootTargetOffset = IKFootTrace(IKTraceDistance, RightFootSocket);
	const float LeftFootTargetOffset = IKFootTrace(IKTraceDistance, LeftFootSocket);

	CurrentRightFootOffset = FMath::FInterpTo(CurrentRightFootOffset, RightFootTargetOffset, DeltaTime, IKInterpSpeed);
	CurrentLeftFootOffset = FMath::FInterpTo(CurrentLeftFootOffset, LeftFootTargetOffset, DeltaTime, IKInterpSpeed);
}

void ABaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	IKTraceDistance = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void ABaseCharacter::OnWalkSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	GetCharacterMovement()->MaxWalkSpeed = OnAttributeChangeData.NewValue;
}

void ABaseCharacter::OnCurrentHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (OnAttributeChangeData.NewValue <= 0.f)
	{
		OnZeroHealth();
	}
}

void ABaseCharacter::OnZeroHealth()
{
	if (GetController() && GetController()->IsA<AAIController>())
	{
		GetController()->UnPossess();
	}

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionResponseToChannel(BULLET_TRACE_COLLISION, ECR_Ignore);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(this))
	{
		auto AbilitySystemComponent = Cast<UFPSAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent());
		if (!AbilitySystemComponent)
		{
			return;
		}

		AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Character.State.Dead"));
	}
}

float ABaseCharacter::IKFootTrace(float TraceDistance, FName Bone)
{
	FVector BoneLocation = GetMesh()->GetSocketLocation(Bone);
	FVector ActorLocation = GetActorLocation();
	FVector StartTrace = FVector{ BoneLocation.X, BoneLocation.Y, ActorLocation.Z };
	FVector EndTrace = FVector{ BoneLocation.X, BoneLocation.Y, ActorLocation.Z - TraceDistance };

	ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;

	bool bHit = UKismetSystemLibrary::LineTraceSingle(this, StartTrace, EndTrace, TraceTypeQuery, true, ActorsToIgnore, EDrawDebugTrace::None,
													  HitResult, true);

	FVector DifferenceVector = EndTrace - HitResult.Location;
	float IKOffset = DifferenceVector.Length() / GetActorScale().X;

	return bHit ? IKOffset : 0.f;
}

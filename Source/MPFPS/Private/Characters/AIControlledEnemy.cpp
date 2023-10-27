// Copyright Andrei Bondarenko 2023


#include "Characters/AIControlledEnemy.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "GameplayAbilitySystem/AttributeSets/BaseAttributeSet.h"

DEFINE_LOG_CATEGORY_STATIC(LogAIControlledCharacter, All, All);

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

void AAIControlledEnemy::Death()
{
	Super::Death();

	bDissolve = true;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		Destroy(true);
	}, 3.f, false);
}

void AAIControlledEnemy::BeginPlay()
{
	Super::BeginPlay();

	CreateDynamicMaterials();
	GrantAbilities();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		InitializeAttributes();

		// enemies placed on a level that are dead when a new client connected should be destroyed on that client
		auto AttributeSet = AbilitySystemComponent->GetAttributeSet<UBaseAttributeSet>();
		if (AttributeSet)
		{
			if (AttributeSet->GetCurrentHealth() <= 0.f) // todo: check for a tag instead
			{
				Destroy(true);
			}
		}
	}
}

void AAIControlledEnemy::CreateDynamicMaterials()
{
	TArray<UMaterialInterface*> Materials = GetMesh()->GetMaterials();

	int32 MaterialCount = 0;
	for (auto* Material : Materials)
	{
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
		if (DynamicMaterial)
		{
			DynamicMaterials.Add(DynamicMaterial);
			GetMesh()->SetMaterial(MaterialCount, DynamicMaterial);
			DissolveValues.Add(1.f);
		}
		else
		{
			UE_LOG(LogAIControlledCharacter, Error, TEXT("Couldn't create a dynamic material"))
		}

		++MaterialCount;
	}
}

void AAIControlledEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bDissolve)
	{
		int32 MaterialCount = 0;
		for (auto* Material : DynamicMaterials)
		{
			float DissolveValue = DissolveValues[MaterialCount];
			DissolveValue = FMath::FInterpConstantTo(DissolveValue, 0.f, DeltaSeconds, DissolveSpeed);
			Material->SetScalarParameterValue("Appearance", DissolveValue);

			DissolveValues[MaterialCount] = DissolveValue;
			++MaterialCount;
		}
	}
}


// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Calculations/BulletDamage.h"

#include "Characters/BaseCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "GameplayAbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "GameplayFramework/FPSGameState.h"
#include "Settings/FPSSessionSettings.h"
#include "Weapons/EquipmentComponent.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);

	FDamageStatics() { DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CurrentHealth, Target, false); }
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DStatics;
	return DStatics;
}

UBulletDamage::UBulletDamage()
{
	RelevantAttributesToCapture.Add(DamageStatics().CurrentHealthDef);
}

void UBulletDamage::CalculateFriendlyFireDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec,
												float& OutDamage) const
{
	auto TeamAgentInterface = Cast<IGenericTeamAgentInterface>(ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor());
	if (!TeamAgentInterface)
	{
		return;
	}

	AActor* EffectCauser = Spec.GetEffectContext().GetEffectCauser();
	if (!EffectCauser)
	{
		return;
	}

	if (TeamAgentInterface->GetTeamAttitudeTowards(*EffectCauser) == ETeamAttitude::Friendly)
	{
		auto World = ExecutionParams.GetTargetAbilitySystemComponent()->GetWorld();
		if (World)
		{
			auto GameState = World->GetGameState<AFPSGameState>();
			if (GameState)
			{
				if (GameState->GetSessionSettings())
				{
					OutDamage *= GameState->GetSessionSettings()->FriendlyFireDamageMultiplier;
				}
			}
		}
	}
}

void UBulletDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
										   FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FHitResult* Hit = Spec.GetContext().GetHitResult();

	float Damage = 0.f;

	if (ensureAlwaysMsgf(Hit, TEXT("HitResult is not valid in BulletDamage execution")))
	{
		AActor* EffectCauser = Spec.GetEffectContext().GetEffectCauser();
		check(EffectCauser);
		if (auto EffectCauserEquipment = EffectCauser->FindComponentByClass<UEquipmentComponent>())
		{
			const FWeaponStats WeaponStats = EffectCauserEquipment->GetWeaponStats();

			auto TargetCharacter = Cast<ABaseCharacter>(ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor());
			if (TargetCharacter)
			{
				if (TargetCharacter->GetHeadBones().Contains(Hit->BoneName))
				{
					Damage = WeaponStats.HeadshotDamage;
				}
				else if (TargetCharacter->GetLegBones().Contains(Hit->BoneName))
				{
					Damage = WeaponStats.LegshotDamage;
				}
				else
				{
					Damage = WeaponStats.DamagePerBullet;
				}
			}
		}
	}

	CalculateFriendlyFireDamage(ExecutionParams, Spec, Damage);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().CurrentHealthProperty, EGameplayModOp::Additive, -Damage));
}

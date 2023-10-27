// Copyright Andrei Bondarenko 2023


#include "GameplayAbilitySystem/Calculations/BulletDamage.h"

#include "Characters/BaseCharacter.h"
#include "GameplayAbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "Weapons/EquipmentComponent.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CurrentHealth, Target, false);
	}
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

void UBulletDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FHitResult* Hit = Spec.GetContext().GetHitResult();

	float Damage = 0.f;

	if (ensureAlwaysMsgf(Hit, TEXT("HitResult is not valid in BulletDamage execution")))
	{
		AActor* EffectCauser  = Spec.GetEffectContext().GetEffectCauser();
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

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().CurrentHealthProperty, EGameplayModOp::Additive, -Damage));
}

// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GA_AimDownSights.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Characters/PlayerCharacter.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_WaitChangeFOV.h"
#include "Weapons/EquipmentComponent.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAimDownSightsAbility, All, All);

void UGA_AimDownSights::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
										const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	OwnerCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor);
	if (!OwnerCharacter)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		UE_LOG(LogAimDownSightsAbility, Error, TEXT("AimDownSights ability activated on non-APlayerCharacter actor"))
		return;
	}

	if (!OwnerCharacter->GetEquipmentComponent()->GetCurrentWeapon())
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	InitialFOV = OwnerCharacter->GetFirstPersonCamera()->FieldOfView;

	if (AimingEffect)
	{
		FGameplayEffectSpecHandle AimingEffectSpec = MakeOutgoingGameplayEffectSpec(AimingEffect);
		ActiveAimingEffectSpec = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, AimingEffectSpec);
	}

	WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	if (!WaitInputReleaseTask)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		UE_LOG(LogAimDownSightsAbility, Error, TEXT("Couldn't create WaitInputReleaseTask"))
		return;
	}

	WaitInputReleaseTask->OnRelease.AddDynamic(this, &UGA_AimDownSights::OnInputRelease);
	WaitInputReleaseTask->ReadyForActivation();

	if (ActorInfo->IsLocallyControlled())
	{
		WaitChangeFOVTask =
			UAbilityTask_WaitChangeFOV::WaitChangeFOV(this, NAME_None, OwnerCharacter->GetFirstPersonCamera(), TargetFOV, InterpSpeed);
		if (!WaitChangeFOVTask)
		{
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
			UE_LOG(LogAimDownSightsAbility, Error, TEXT("Couldn't create WaitChangeFOVTask"))
			return;
		}

		WaitChangeFOVTask->OnTargetFOVReached.AddDynamic(this, &UGA_AimDownSights::OnTargetFOVReached);
		WaitChangeFOVTask->OnChangeFOVTick.AddDynamic(this, &UGA_AimDownSights::ChangeFOVTick);
		WaitChangeFOVTask->ReadyForActivation();
	}
}

void UGA_AimDownSights::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									 const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (WaitChangeFOVTask)
	{
		WaitChangeFOVTask->EndTask();
	}

	WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &UGA_AimDownSights::OnInputRelease);
	WaitInputReleaseTask->ReadyForActivation();

	if (ActorInfo->IsLocallyControlled())
	{
		WaitChangeFOVTask =
			UAbilityTask_WaitChangeFOV::WaitChangeFOV(this, NAME_None, OwnerCharacter->GetFirstPersonCamera(), TargetFOV, InterpSpeed);
		WaitChangeFOVTask->OnTargetFOVReached.AddDynamic(this, &UGA_AimDownSights::OnTargetFOVReached);
		WaitChangeFOVTask->OnChangeFOVTick.AddDynamic(this, &UGA_AimDownSights::ChangeFOVTick);
		WaitChangeFOVTask->ReadyForActivation();
	}
}

void UGA_AimDownSights::OnTargetFOVReached()
{
}

void UGA_AimDownSights::ChangeFOVTick(float DeltaTime, float CurrentFOV)
{
	const float FOVAlpha = (CurrentFOV - TargetFOV) / (InitialFOV - TargetFOV);

	UEquipmentComponent* Equipment = OwnerCharacter->GetEquipmentComponent();
	UWeapon* Weapon = Equipment->GetCurrentWeapon();

	FVector IntermediateLocation =
		Weapon->ArmsMeshRelativeLocationWhenAiming + FOVAlpha * (Weapon->ArmsMeshRelativeLocation - Weapon->ArmsMeshRelativeLocationWhenAiming);

	OwnerCharacter->GetFirstPersonMesh()->SetRelativeLocation(IntermediateLocation);
}

void UGA_AimDownSights::OnInitialFOVReached()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_AimDownSights::OnInputRelease(float TimeHeld)
{
	if (GetCurrentActorInfo()->IsLocallyControlled())
	{
		if (WaitChangeFOVTask)
		{
			WaitChangeFOVTask->EndTask();
		}

		WaitChangeFOVTask =
			UAbilityTask_WaitChangeFOV::WaitChangeFOV(this, NAME_None, OwnerCharacter->GetFirstPersonCamera(), InitialFOV, InterpSpeed);
		WaitChangeFOVTask->OnTargetFOVReached.AddDynamic(this, &UGA_AimDownSights::OnInitialFOVReached);
		WaitChangeFOVTask->ReadyForActivation();
	}

	UAbilitySystemComponent* const AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Ensured();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveAimingEffectSpec);
	}
}

// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GA_Reload.h"
#include "Characters/PlayerCharacter.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_PlayMontageForMesh.h"
#include "Weapons/EquipmentComponent.h"
#include "Weapons/Weapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogReloadAbility, All, All);

void UGA_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	auto PlayerCharacter = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!PlayerCharacter)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageForMesh* PlayThirdPersonMontageTask = UAbilityTask_PlayMontageForMesh::PlayMontageForMeshAndWaitForEvent(
		this, NAME_None, PlayerCharacter->GetMesh(), ThirdPersonReloadMontage, FGameplayTagContainer(), 1.f, NAME_None, false);

	PlayThirdPersonMontageTask->ReadyForActivation();

	FGameplayTagContainer EventTags;
	EventTags.AddTag(ReloadEventTag);
	PlayFirstPersonMontage = UAbilityTask_PlayMontageForMesh::PlayMontageForMeshAndWaitForEvent(
		this, NAME_None, PlayerCharacter->GetFirstPersonMesh(), FirstPersonReloadMontage, EventTags, 1.f, NAME_None, false, 1.f, false);

	if (!PlayFirstPersonMontage)
	{
		UE_LOG(LogReloadAbility, Error, TEXT("Can't create UAbilityTask_PlayMontageForMesh. Reaload ability is cancelled."))
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	PlayFirstPersonMontage->OnInterrupted.AddDynamic(this, &UGA_Reload::OnMontageCancelled);
	PlayFirstPersonMontage->OnCancelled.AddDynamic(this, &UGA_Reload::OnMontageCancelled);
	PlayFirstPersonMontage->OnCompleted.AddDynamic(this, &UGA_Reload::OnMontageCompleted);
	PlayFirstPersonMontage->EventReceived.AddDynamic(this, &UGA_Reload::OnEventReceived);

	PlayFirstPersonMontage->ReadyForActivation();
}

bool UGA_Reload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
									FGameplayTagContainer* OptionalRelevantTags) const
{
	if (auto EquipmentComponent = ActorInfo->AvatarActor->FindComponentByClass<UEquipmentComponent>())
	{
		const bool bHasWeaponInHands = EquipmentComponent->GetCurrentItem()->IsA<UWeapon>();
		const bool bHasReserveAmmo = EquipmentComponent->GetCurrentReserveAmmo() > 0.f;
		const bool bMaxClipAmmo = EquipmentComponent->IsMaxClipAmmo();

		return bHasReserveAmmo && bHasWeaponInHands && !bMaxClipAmmo;
	}

	return false;
}

void UGA_Reload::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UGA_Reload::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Reload::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UGA_Reload::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_Reload::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag.MatchesTag(FGameplayTag::RequestGameplayTag("Weapon.Event.Reload")))
	{
		UEquipmentComponent* EquipmentComponent = GetCurrentActorInfo()->AvatarActor->FindComponentByClass<UEquipmentComponent>();
		if (!EquipmentComponent)
		{
			PlayFirstPersonMontage->EventReceived.RemoveDynamic(this, &UGA_Reload::OnEventReceived);
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
			return;
		}

		EquipmentComponent->ReloadAmmo();
	}
}

// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GA_GiveWeapon.h"
#include "Characters/PlayerCharacter.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_PlayMontageForMesh.h"
#include "Weapons/EquipmentComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGiveWeaponAbility, All, All);

void UGA_GiveWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (auto PlayerCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor))
	{
		PlayerCharacter->GetEquipmentComponent()->AddWeapon(Weapon);

		UAbilityTask_PlayMontageForMesh* PlayThirdPersonMontageTask = UAbilityTask_PlayMontageForMesh::PlayMontageForMeshAndWaitForEvent(
			this, NAME_None, PlayerCharacter->GetMesh(), ThirdPersonMontage, FGameplayTagContainer(), 1.f, NAME_None, false);

		PlayThirdPersonMontageTask->ReadyForActivation();


		PlayFirstPersonMontageTask = UAbilityTask_PlayMontageForMesh::PlayMontageForMeshAndWaitForEvent(
			this, NAME_None, PlayerCharacter->GetFirstPersonMesh(), FirstPersonMontage, FGameplayTagContainer(), 1.f, NAME_None, false, 1.f, false);

		if (!PlayFirstPersonMontageTask)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}

		PlayFirstPersonMontageTask->OnInterrupted.AddDynamic(this, &UGA_GiveWeapon::OnMontageCompleted);
		PlayFirstPersonMontageTask->OnCancelled.AddDynamic(this, &UGA_GiveWeapon::OnMontageCompleted);
		PlayFirstPersonMontageTask->OnCompleted.AddDynamic(this, &UGA_GiveWeapon::OnMontageCompleted);

		PlayFirstPersonMontageTask->ReadyForActivation();
	}
	else
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		UE_LOG(LogGiveWeaponAbility, Error, TEXT("%s ability tried to activate on non-PlayerCharacter actor"), *GetName())
	}
}

void UGA_GiveWeapon::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GA_FireOnce.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Camera/CameraComponent.h"
#include "Characters/ShootingCharacter.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_PlayMontageForMesh.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_ServerWaitForClientData.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_WaitTargetDataUsingActor.h"
#include "GameplayAbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "GameplayAbilitySystem/TargetActors/TargetActor_LineTrace.h"
#include "Types/CollisionTypes.h"
#include "Weapons/EquipmentComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFireOnceAbility, All, All);

void UGA_FireOnce::FireShot()
{
	if (GetActorInfo().IsLocallyControlledPlayer())
	{
		if (!TargetActor)
		{
			SpawnTargetActor();
		}

		auto LineTraceActor = Cast<ATargetActor_LineTrace>(TargetActor);
		if (LineTraceActor)
		{
			auto Character = Cast<AShootingCharacter>(GetSourceObject(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo()));
			check(Character);

			const FVector StartTrace = Character->GetFirstPersonCamera()->GetComponentLocation();
			const float Spread = Character->GetEquipmentComponent()->GetSpread();
			const float HalfConeAngleDeg = FMath::Lerp(0.01, 20.f, Spread);
			const FVector DeviationVector = FMath::VRandCone(Character->GetBaseAimRotation().Vector(), FMath::DegreesToRadians(HalfConeAngleDeg));
			const FVector EndTrace = StartTrace + DeviationVector * TraceLength;

			LineTraceActor->Configure(StartTrace, EndTrace, BULLET_TRACE_COLLISION);
		}

		auto PlayerCharacter = Cast<AShootingCharacter>(GetActorInfo().AvatarActor);
		if (!PlayerCharacter)
		{
			UE_LOG(LogFireOnceAbility, Error, TEXT("%s ability tried to activate on non-AShootingCharacter actor"));
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
			return;
		}

		auto EquipmentComponent = PlayerCharacter->GetEquipmentComponent();
		if (EquipmentComponent)
		{
			EquipmentComponent->AddSpread();
		}

		if (ThirdPersonFireMontage)
		{
			UAbilityTask_PlayMontageForMesh* PlayThirdPersonMontageTask = UAbilityTask_PlayMontageForMesh::PlayMontageForMeshAndWaitForEvent(
				this, NAME_None, PlayerCharacter->GetMesh(), ThirdPersonFireMontage, FGameplayTagContainer(), 1.f, NAME_None, false);

			PlayThirdPersonMontageTask->ReadyForActivation();
		}

		if (FirstPersonFireMontage && FirstPersonFireMontageAiming)
		{
			bool bAiming = GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Weapon.State.Aiming"));
			UAnimMontage* MontageToPlay = bAiming ? FirstPersonFireMontageAiming : FirstPersonFireMontage;

			UAbilityTask_PlayMontageForMesh* PlayFirstPersonMontageTask = UAbilityTask_PlayMontageForMesh::PlayMontageForMeshAndWaitForEvent(
				this, NAME_None, PlayerCharacter->GetFirstPersonMesh(), MontageToPlay, FGameplayTagContainer(), 1.f, NAME_None, false);

			PlayFirstPersonMontageTask->ReadyForActivation();
		}

		WaitTargetDataTask = UAbilityTask_WaitTargetDataUsingActor::WaitTargetDataWithReusableActor(
			this, NAME_None, EGameplayTargetingConfirmation::Instant, TargetActor, true);

		WaitTargetDataTask->ValidData.AddDynamic(this, &UGA_FireOnce::OnValidDataAcquired);
		WaitTargetDataTask->ReadyForActivation();
	}
}

void UGA_FireOnce::SpawnTargetActor()
{
	if (!TargetActor)
	{
		AGameplayAbilityTargetActor* SpawnedActor = nullptr;

		UAbilityTask_WaitTargetData* SpawnTargetActorTask = UAbilityTask_WaitTargetData::WaitTargetData(
			this, NAME_None, EGameplayTargetingConfirmation::Instant, ATargetActor_LineTrace::StaticClass());

		SpawnTargetActorTask->BeginSpawningActor(this, ATargetActor_LineTrace::StaticClass(), SpawnedActor);
		SpawnTargetActorTask->FinishSpawningActor(this, SpawnedActor);

		TargetActor = SpawnedActor;
	}
}

void UGA_FireOnce::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
												const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	SpawnTargetActor();

	ServerWaitForClientDataTask = UAbilityTask_ServerWaitForClientData::ServerWaitForClientTargetData(this, NAME_None, false);
	ServerWaitForClientDataTask->ValidData.AddDynamic(this, &UGA_FireOnce::OnValidDataAcquired);
	ServerWaitForClientDataTask->ReadyForActivation();

	FireShot();
}

bool UGA_FireOnce::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
										  FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!GetCurrentActorInfo())
	{
		return false;
	}

	if (!GetCurrentActorInfo()->AvatarActor.IsValid())
	{
		return false;
	}

	if (auto EquipmentComponent = GetCurrentActorInfo()->AvatarActor->FindComponentByClass<UEquipmentComponent>())
	{
		return EquipmentComponent->GetCurrentClipAmmo() > 0.f;
	}

	return false;
}

bool UGA_FireOnce::CommitAbilityCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
												  const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags)
{
	if (auto EquipmentComponent = GetCurrentActorInfo()->AvatarActor->FindComponentByClass<UEquipmentComponent>())
	{
		EquipmentComponent->SpendAmmo(1.f);
	}

	return true;
}

void UGA_FireOnce::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
										   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ServerWaitForClientDataTask)
	{
		ServerWaitForClientDataTask->EndTask();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_FireOnce::GiveRewardToInstigator(UAbilitySystemComponent* TargetAbilitySystemComponent)
{
	auto BaseAttributeSet = Cast<UBaseAttributeSet>(TargetAbilitySystemComponent->GetAttributeSet(UBaseAttributeSet::StaticClass()));
	if (BaseAttributeSet->GetCurrentHealth() == 0.f && GetCurrentActorInfo()->IsNetAuthority() &&
		TargetAbilitySystemComponent->GetAvatarActor() != LastDeadTarget)
	{
		LastDeadTarget = TargetAbilitySystemComponent->GetAvatarActor();

		auto TargetCharacter = Cast<ABaseCharacter>(TargetAbilitySystemComponent->GetAvatarActor());
		{
			float Reward = TargetCharacter->GetReward();

			FGameplayEffectSpecHandle AddMoneyEffectSpec = MakeOutgoingGameplayEffectSpec(AddMoneyEffect);
			AddMoneyEffectSpec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Value.Money"), Reward);

			GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*AddMoneyEffectSpec.Data.Get());
		}
	}
}

void UGA_FireOnce::OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data)
{
	CommitAbilityCost(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), nullptr);

	const FGameplayAbilityTargetData* DataPtr = Data.Get(0);
	if (DataPtr)
	{
		FGameplayCueParameters Parameters;
		Parameters.EffectCauser = GetAvatarActorFromActorInfo();
		Parameters.Location = DataPtr->GetEndPoint();

		auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(DataPtr->GetHitResult()->GetActor());
		if (AbilitySystemInterface)
		{
			// apply damage to the target
			UAbilitySystemComponent* TargetAbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();

			const FGameplayEffectSpecHandle EffectSpec = MakeOutgoingGameplayEffectSpec(DamageEffect);

			Parameters.EffectContext = EffectSpec.Data.Get()->GetEffectContext();
			Parameters.EffectContext.AddHitResult(*DataPtr->GetHitResult());

			TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());

			GiveRewardToInstigator(TargetAbilitySystemComponent);
		}

		auto AbilitySystemComponent = Cast<UFPSAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->ExecuteGameplayCue(ShotGameplayCue, Parameters);
		}

		LastShotTime = GetWorld()->GetTimeSeconds();
	}
}

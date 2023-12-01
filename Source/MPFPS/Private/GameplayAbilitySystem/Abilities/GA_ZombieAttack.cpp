// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/Abilities/GA_ZombieAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "GameplayAbilitySystem/AbilityTasks/AbilityTask_PlayMontage.h"
#include "GameplayAbilitySystem/TargetActors/TargetActor_BoxCollision.h"
#include "Types/CollisionTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogZombieBiteAbility, All, All);

UGA_ZombieAttack::UGA_ZombieAttack()
{
	CollisionChannel = ONLY_PLAYER_COLLISION;
}

void UGA_ZombieAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (AttackMontages.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("AttackMontages is empty in %s"), *GetName())
		Cancel();
		return;
	}

	int32 RandomIndex = 0;
	if (AttackMontages.Num() != 1)
	{
		RandomIndex = FMath::Rand() % AttackMontages.Num();
	}

	UAnimMontage* AttackMontageToPlay = AttackMontages[RandomIndex];
	if (!AttackMontageToPlay)
	{
		UE_LOG(LogZombieBiteAbility, Error, TEXT("AttackMontage at index %i is null"), RandomIndex)
		Cancel();
		return;
	}

	FGameplayTagContainer EventTags;
	EventTags.AddTag(AttackEventTag);
	UAbilityTask_PlayMontage* PlayMontageTask =
		UAbilityTask_PlayMontage::PlayMontageAndWaitForEvent(this, NAME_None, AttackMontageToPlay, EventTags, AttackSpeed);
	if (!PlayMontageTask)
	{
		UE_LOG(LogZombieBiteAbility, Error, TEXT("Couldn't create UAbilityTask_PlayMontage"))
		Cancel();
		return;
	}

	PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_ZombieAttack::OnMontageCancelled);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_ZombieAttack::OnMontageCancelled);

	PlayMontageTask->EventReceived.AddDynamic(this, &UGA_ZombieAttack::OnAnimEventReceived);

	PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_ZombieAttack::OnMontageCompleted);

	PlayMontageTask->ReadyForActivation();
}

void UGA_ZombieAttack::Cancel()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UGA_ZombieAttack::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Cancel();
}

void UGA_ZombieAttack::OnValidDataAcquired(const FGameplayAbilityTargetDataHandle& Data)
{
	if (GetCurrentActorInfo()->AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Character.State.Dead")))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}

	const FGameplayAbilityTargetData* TargetData = Data.Get(0);
	if (TargetData)
	{
		if (auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(TargetData->GetHitResult()->GetActor()))
		{
			if (!DamageEffect)
			{
				UE_LOG(LogZombieBiteAbility, Error, TEXT("DamageEffect is null in %s"), *GetName())
				Cancel();
				return;
			}

			UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();

			FGameplayEffectSpecHandle DamageEffectSpec = MakeOutgoingGameplayEffectSpec(DamageEffect);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageEffectSpec.Data.Get());

			FGameplayCueParameters GameplayCueParameters;
			GameplayCueParameters.EffectCauser = GetAvatarActorFromActorInfo();
			AbilitySystemComponent->ExecuteGameplayCue(GameplayCueTag, GameplayCueParameters);
		}
	}
}

void UGA_ZombieAttack::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_ZombieAttack::OnAnimEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag.MatchesTag(FGameplayTag::RequestGameplayTag("Zombie.Event.Attack")))
	{
		UAbilityTask_WaitTargetData* SpawnTargetActorTask = UAbilityTask_WaitTargetData::WaitTargetData(
			this, NAME_None, EGameplayTargetingConfirmation::Instant, ATargetActor_BoxCollision::StaticClass());

		SpawnTargetActorTask->ValidData.AddDynamic(this, &UGA_ZombieAttack::OnValidDataAcquired);

		AGameplayAbilityTargetActor* SpawnedActor;
		SpawnTargetActorTask->BeginSpawningActor(this, ATargetActor_BoxCollision::StaticClass(), SpawnedActor);

		if (auto BoxCollisionTargetActor = Cast<ATargetActor_BoxCollision>(SpawnedActor))
		{
			TWeakObjectPtr<AActor> AvatarActor = GetCurrentActorInfo()->AvatarActor;
			check(AvatarActor.IsValid());

			const FVector StartTrace = AvatarActor->GetActorLocation();
			const FVector EndTrace = AvatarActor->GetActorForwardVector() * AttackRange + StartTrace;
			TArray<AActor*> ActorsToIgnore = { AvatarActor.Get() };

			BoxCollisionTargetActor->Configure(StartTrace, EndTrace, ActorsToIgnore, ONLY_PLAYER_COLLISION, HalfSize,
											   AvatarActor->GetActorRotation());
		}

		SpawnTargetActorTask->FinishSpawningActor(this, SpawnedActor);
	}
}

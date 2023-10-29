// Copyright Andrei Bondarenko 2023

#include "AI/Tasks/BTTask_ActivateGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"

DEFINE_LOG_CATEGORY_STATIC(LogBTTaskActivateAbility, All, All);

UBTTask_ActivateGameplayAbility::UBTTask_ActivateGameplayAbility()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_ActivateGameplayAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto OwnerController = OwnerComp.GetOwner<AAIController>();
	if (!OwnerController || !AbilityToActivate)
	{
		return EBTNodeResult::Aborted;
	}

	if (auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(OwnerController->GetPawn()))
	{
		AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
		FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(AbilityToActivate);
		if (!AbilitySpec)
		{
			UE_LOG(LogBTTaskActivateAbility, Error, TEXT("Couldn't find ability spec with class %s"), *AbilityToActivate->GetName())
			return EBTNodeResult::Failed;
		}

		bool bActivated = AbilitySystemComponent->TryActivateAbility(AbilitySpec->Handle);

		AbilityDoneDelegateHandle =
			AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &UBTTask_ActivateGameplayAbility::OnAbilityEnded, &OwnerComp);

		if (bActivated)
		{
			bool bInstanced;
			const UGameplayAbility* Ability =
				UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(AbilitySystemComponent, AbilitySpec->Handle, bInstanced);
			ActivatedAbility = const_cast<UGameplayAbility*>(Ability);

			return EBTNodeResult::InProgress;
		}

		return EBTNodeResult::Failed;
	}
	else
	{
		return EBTNodeResult::Aborted;
	}
}

void UBTTask_ActivateGameplayAbility::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	AbilitySystemComponent->OnAbilityEnded.Remove(AbilityDoneDelegateHandle);
}

void UBTTask_ActivateGameplayAbility::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskDeactivated(Task);

	AbilitySystemComponent->OnAbilityEnded.Remove(AbilityDoneDelegateHandle);
}

void UBTTask_ActivateGameplayAbility::OnAbilityEnded(const FAbilityEndedData& AbilityEndedData, UBehaviorTreeComponent* OwnerComp)
{
	if (AbilityEndedData.AbilityThatEnded->GetClass() == ActivatedAbility->GetClass())
	{
		AbilitySystemComponent->OnAbilityEnded.Remove(AbilityDoneDelegateHandle);
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
}

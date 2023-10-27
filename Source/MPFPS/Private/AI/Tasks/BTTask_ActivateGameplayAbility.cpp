// Copyright Andrei Bondarenko 2023


#include "AI/Tasks/BTTask_ActivateGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "AIController.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"

EBTNodeResult::Type UBTTask_ActivateGameplayAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto OwnerController = OwnerComp.GetOwner<AAIController>();
	if (!OwnerController)
	{
		return EBTNodeResult::Aborted;
	}

	if (auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(OwnerController->GetPawn()))
	{
		UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
		bool bActivated = AbilitySystemComponent->TryActivateAbilityByClass(AbilityToActivate);

		return bActivated ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	else
	{
		return EBTNodeResult::Aborted;
	}
}

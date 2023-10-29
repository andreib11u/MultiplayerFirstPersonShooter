// Copyright Andrei Bondarenko 2023


#include "AI/Tasks/BTTask_ApplyEffectToSelf.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_ApplyEffectToSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetOwner<AAIController>();
	if (!AIController)
	{
		return EBTNodeResult::Aborted;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Aborted;
	}

	if (auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(ControlledPawn))
	{
		auto AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();

		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(EffectToApply, 1, EffectContext);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

// Copyright Andrei Bondarenko 2023

#include "AI/Tasks/BTTask_FindCloseRandomLocation.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_FindCloseRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetOwner<AAIController>();
	if (AIController)
	{
		APawn* ControlledPawn = AIController->GetPawn();
		if (ControlledPawn)
		{
			FNavLocation ResultLocation;
			UNavigationSystemV1::GetNavigationSystem(GetWorld())
				->GetRandomPointInNavigableRadius(ControlledPawn->GetActorLocation(), Range, ResultLocation);

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ResultLocationKey.SelectedKeyName, ResultLocation);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

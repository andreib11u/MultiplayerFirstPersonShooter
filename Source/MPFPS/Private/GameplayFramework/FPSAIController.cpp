// Copyright Andrei Bondarenko 2023


#include "GameplayFramework/FPSAIController.h"

void AFPSAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTreeToRun);
}

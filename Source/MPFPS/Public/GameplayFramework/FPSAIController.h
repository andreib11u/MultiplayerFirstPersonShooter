// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPSAIController.generated.h"

/**
 * 
 */
UCLASS()
class MPFPS_API AFPSAIController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTreeToRun;
};

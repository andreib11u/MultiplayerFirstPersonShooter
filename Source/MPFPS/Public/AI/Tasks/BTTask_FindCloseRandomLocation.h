// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindCloseRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class MPFPS_API UBTTask_FindCloseRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	float Range = 200.f;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ResultLocationKey;
};

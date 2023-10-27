// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateGameplayAbility.generated.h"

class UFPSGameplayAbility;
/**
 * 
 */
UCLASS()
class MPFPS_API UBTTask_ActivateGameplayAbility : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UFPSGameplayAbility> AbilityToActivate;
};

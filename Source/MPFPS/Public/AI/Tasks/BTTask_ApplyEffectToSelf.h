// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ApplyEffectToSelf.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class MPFPS_API UBTTask_ApplyEffectToSelf : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> EffectToApply;
};

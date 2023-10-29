// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
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
	UBTTask_ActivateGameplayAbility();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UFPSGameplayAbility> AbilityToActivate;

	FDelegateHandle AbilityDoneDelegateHandle;
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UGameplayAbility* ActivatedAbility;

	void OnAbilityEnded(const FAbilityEndedData& AbilityEndedData, UBehaviorTreeComponent* OwnerComp);
};

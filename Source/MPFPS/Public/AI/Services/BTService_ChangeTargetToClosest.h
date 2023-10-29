// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ChangeTargetToClosest.generated.h"

class UFindActorsOfClassSubsystem;
/**
 * 
 */
UCLASS()
class MPFPS_API UBTService_ChangeTargetToClosest : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_ChangeTargetToClosest();

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY()
	UFindActorsOfClassSubsystem* FindActorsSubsystem;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetEnemyKey;

	UPROPERTY()
	TMap<AActor*, float> Distances;
};

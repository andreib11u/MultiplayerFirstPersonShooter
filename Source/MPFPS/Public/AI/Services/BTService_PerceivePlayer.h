// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_PerceivePlayer.generated.h"

class UFindActorsOfClassSubsystem;
/**
 * 
 */
UCLASS()
class MPFPS_API UBTService_PerceivePlayer : public UBTService
{
	GENERATED_BODY()
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
private:
	UPROPERTY()
	UFindActorsOfClassSubsystem* FindActorsSubsystem;

	UPROPERTY(EditAnywhere)
	float SeeingDistance = 2000.f;
	UPROPERTY(EditAnywhere, meta = (Units = "Degrees", ClampMax = "179", ClampMin = "10"))
	float HalfLineOfSightAngle = 45.f;
	UPROPERTY(EditAnywhere)
	float HearingDistance = 400.f;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector EnemyKey;

	// for optimized distance checks:

	float SeeingDistanceSquared;
	float HearingDistanceSquared;
};

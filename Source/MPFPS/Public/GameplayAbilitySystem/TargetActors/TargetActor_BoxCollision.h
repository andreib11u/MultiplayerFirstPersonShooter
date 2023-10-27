// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TargetActor_BoxCollision.generated.h"

/**
 *
 */
UCLASS()
class MPFPS_API ATargetActor_BoxCollision : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	ATargetActor_BoxCollision();

	void Configure(const FVector& InStartTrace, const FVector& InEndTrace, const TArray<AActor*>& InActorsToIgnore,
				   ECollisionChannel InCollisionChannel, const FVector& InHalfSize, const FRotator& InOrientation);

	TArray<FHitResult> PerformTrace();
	virtual void ConfirmTargetingAndContinue() override;
	FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<FHitResult>& HitResults) const;

private:
	FVector StartTrace;
	FVector EndTrace;
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
	ECollisionChannel CollisionChannel;
	FVector HalfSize;
	FRotator Orientation;
};

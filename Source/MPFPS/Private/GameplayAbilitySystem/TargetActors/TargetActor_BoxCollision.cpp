// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/TargetActors/TargetActor_BoxCollision.h"
#include "Abilities/GameplayAbility.h"
#include "Characters/BaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CollisionTypes.h"

ATargetActor_BoxCollision::ATargetActor_BoxCollision()
{
	ShouldProduceTargetDataOnServer = true;
}

void ATargetActor_BoxCollision::Configure(const FVector& InStartTrace, const FVector& InEndTrace, const TArray<AActor*>& InActorsToIgnore,
										  ECollisionChannel InCollisionChannel, const FVector& InHalfSize, const FRotator& InOrientation)
{
	StartTrace = InStartTrace;
	EndTrace = InEndTrace;
	ActorsToIgnore = InActorsToIgnore;
	CollisionChannel = InCollisionChannel;
	HalfSize = InHalfSize;
	Orientation = InOrientation;
}

TArray<FHitResult> ATargetActor_BoxCollision::PerformTrace()
{
	TArray<FHitResult> Results;

	const ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(CollisionChannel);
	TArray<ETraceTypeQuery> ObjectTypes;
	ObjectTypes.Add(TraceType);

	TArray<FHitResult> LineTraceMultiResults;
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), EndTrace, EndTrace, HalfSize, Orientation,
										TraceType, true, ActorsToIgnore, EDrawDebugTrace::None, LineTraceMultiResults, true,
										FLinearColor::Yellow);

	// make hits from LineTraceMulti unique
	TArray<AActor*> HitActors;
	for (FHitResult MultiResult : LineTraceMultiResults)
	{
		if (!HitActors.Contains(MultiResult.GetActor()))
		{
			Results.Add(MultiResult);
			HitActors.Add(MultiResult.GetActor());
		}
	}

	if (Results.IsEmpty())
	{
		FHitResult NoHitHappenedHitResult = FHitResult();
		NoHitHappenedHitResult.TraceStart = StartTrace;
		NoHitHappenedHitResult.TraceEnd = EndTrace;
		NoHitHappenedHitResult.Location = EndTrace;
		Results.Add(NoHitHappenedHitResult);
	}

	return Results;
}

void ATargetActor_BoxCollision::ConfirmTargetingAndContinue()
{
	if (IsConfirmTargetingAllowed())
	{
		TArray<FHitResult> HitResults = PerformTrace();
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(HitResults);
		TargetDataReadyDelegate.Broadcast(Handle);
	}
}

FGameplayAbilityTargetDataHandle ATargetActor_BoxCollision::MakeTargetData(const TArray<FHitResult>& HitResults) const
{
	FGameplayAbilityTargetDataHandle ReturnDataHandle;

	for (int32 i = 0; i < HitResults.Num(); i++)
	{
		/** Note: These are cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
		FGameplayAbilityTargetData_SingleTargetHit* ReturnData = new FGameplayAbilityTargetData_SingleTargetHit();
		ReturnData->HitResult = HitResults[i];
		ReturnDataHandle.Add(ReturnData);
	}

	return ReturnDataHandle;
}
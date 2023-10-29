// Copyright Andrei Bondarenko 2023

#include "AI/Services/BTService_PerceivePlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Subsystems/FindActorsOfClassSubsystem.h"

void UBTService_PerceivePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const TArray<AActor*> PlayerCharacters = FindActorsSubsystem->FindAllActors(APlayerCharacter::StaticClass());
	for (AActor* PlayerCharacter : PlayerCharacters)
	{
		// check line of sight angle with seeing distance
		auto AIController = OwnerComp.GetOwner<AAIController>();
		APawn* ControlledPawn = AIController->GetPawn();
		FVector MyForwardVector = ControlledPawn->GetActorForwardVector();
		MyForwardVector.Z = 0.f;
		FVector PlayerDirection = PlayerCharacter->GetActorLocation() - ControlledPawn->GetActorLocation();
		PlayerDirection.Z = 0.f;
		PlayerDirection.Normalize();

		const float DotProduct = FVector::DotProduct(MyForwardVector, PlayerDirection);
		const float AngleRadians = FMath::Acos(DotProduct);
		const float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

		const bool bPlayerInLineOfSightAngle = AngleDegrees < HalfLineOfSightAngle;
		const bool bHasLineOfSight = AIController->LineOfSightTo(PlayerCharacter, FVector());

		const float DistanceSquared = FVector::DistSquared2D(ControlledPawn->GetActorLocation(), PlayerCharacter->GetActorLocation());
		const bool bInSeeingDistance = DistanceSquared < SeeingDistanceSquared;

		if (bPlayerInLineOfSightAngle && bHasLineOfSight && bInSeeingDistance)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(EnemyKey.SelectedKeyName, PlayerCharacter);
			return;
		}

		const bool bInHearingDistance = DistanceSquared < HearingDistanceSquared;

		if (bInHearingDistance)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(EnemyKey.SelectedKeyName, PlayerCharacter);
		}
	}
}

void UBTService_PerceivePlayer::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	if (!FindActorsSubsystem)
	{
		FindActorsSubsystem = GetWorld()->GetSubsystem<UFindActorsOfClassSubsystem>();
	}
}

void UBTService_PerceivePlayer::PostLoad()
{
	Super::PostLoad();

	SeeingDistanceSquared = SeeingDistance * SeeingDistance;
	HearingDistanceSquared = HearingDistance * HearingDistance;
}

void UBTService_PerceivePlayer::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UBTService_PerceivePlayer, SeeingDistance))
	{
		SeeingDistanceSquared = SeeingDistance * SeeingDistance;
	}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UBTService_PerceivePlayer, HearingDistance))
	{
		HearingDistanceSquared = HearingDistance * HearingDistance;
	}
}

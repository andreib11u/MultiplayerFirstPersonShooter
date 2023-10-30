// Copyright Andrei Bondarenko 2023


#include "AI/Services/BTService_ChangeTargetToClosest.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Subsystems/FindActorsOfClassSubsystem.h"

UBTService_ChangeTargetToClosest::UBTService_ChangeTargetToClosest()
{
	TargetEnemyKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_ChangeTargetToClosest, TargetEnemyKey), AActor::StaticClass());
}

void UBTService_ChangeTargetToClosest::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	if (!FindActorsSubsystem)
	{
		FindActorsSubsystem = GetWorld()->GetSubsystem<UFindActorsOfClassSubsystem>();
	}
}

void UBTService_ChangeTargetToClosest::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const TArray<AActor*> PlayerCharacters = FindActorsSubsystem->FindAllActors(APlayerCharacter::StaticClass());

	if (PlayerCharacters.IsEmpty())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetEnemyKey.SelectedKeyName, nullptr);
		return;
	}

	for (AActor* PlayerCharacter : PlayerCharacters)
	{
		auto AIController = OwnerComp.GetOwner<AAIController>();
		APawn* ControlledPawn = AIController->GetPawn();

		float Distance = FVector::DistSquared2D(PlayerCharacter->GetActorLocation(), ControlledPawn->GetActorLocation());
		Distances.Add(PlayerCharacter, Distance);
	}

	TArray<AActor*> Actors;
	Distances.GenerateKeyArray(Actors);

	float MinDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;

	for (auto Actor : Actors)
	{
		float Distance = *Distances.Find(Actor);
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestActor = Actor;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetEnemyKey.SelectedKeyName, ClosestActor);
}

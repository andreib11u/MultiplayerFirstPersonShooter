// Copyright Andrei Bondarenko 2023

#include "ZombieSpawner/ZombieSpawn.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subsystems/FindActorsOfClassSubsystem.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Types/CollisionTypes.h"

AZombieSpawn::AZombieSpawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnLocation = CreateDefaultSubobject<USceneComponent>("SpawnLocation");
	SetRootComponent(SpawnLocation);

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("BillboardComponent");
	BillboardComponent->SetupAttachment(SpawnLocation);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(SpawnLocation);
}

bool AZombieSpawn::CanSpawn()
{
	return !bPlayerHasLineOfSight;
}

void AZombieSpawn::BeginPlay()
{
	Super::BeginPlay();

	auto FindActorsSubsystem = GetWorld()->GetSubsystem<UFindActorsOfClassSubsystem>();
	if (FindActorsSubsystem)
	{
		TArray<AActor*> FoundPlayers = FindActorsSubsystem->FindAllActors(APlayerCharacter::StaticClass());

		for (AActor* FoundPlayer : FoundPlayers)
		{
			Players.Add(CastChecked<APlayerCharacter>(FoundPlayer));
		}

		FindActorsSubsystem->OnNewFindableActorAdded.AddUObject(this, &AZombieSpawn::OnNewPlayerAdded);
		FindActorsSubsystem->OnNewFindableActorRemoved.AddUObject(this, &AZombieSpawn::OnNewPlayerRemoved);
	}
}

void AZombieSpawn::CheckPlayerLineOfSight()
{
	bPlayerHasLineOfSight = false;
	for (auto Player : Players)
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		FHitResult HitResult;

		const ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);

		FVector StartTrace = SpawnLocation->GetComponentLocation();
		FVector EndTrace;
		FRotator _;
		Player->GetActorEyesViewPoint(EndTrace, _);

		bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTrace, EndTrace, TraceType, true, ActorsToIgnore, EDrawDebugTrace::None,
														  HitResult, true, FLinearColor::Yellow);

		if (bHit)
		{
			if (Player == HitResult.GetActor())
			{
				bPlayerHasLineOfSight = true;
				break;
			}
		}
	}
}

void AZombieSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPlayerLineOfSight();
}

void AZombieSpawn::OnNewPlayerAdded(UClass* Class, AActor* Actor)
{
	if (Class == APlayerCharacter::StaticClass())
	{
		Players.Add(CastChecked<APlayerCharacter>(Actor));
	}
}

void AZombieSpawn::OnNewPlayerRemoved(UClass* Class, AActor* Actor)
{
	if (Class == APlayerCharacter::StaticClass())
	{
		Players.Remove(CastChecked<APlayerCharacter>(Actor));
	}
}

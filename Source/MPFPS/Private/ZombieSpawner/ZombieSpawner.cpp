// Copyright Andrei Bondarenko 2023

#include "ZombieSpawner/ZombieSpawner.h"
#include "ZombieSpawner/ZombieSpawn.h"
#include "EngineUtils.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/Enemies/Zombie.h"
#include "Subsystems/FindActorsOfClassSubsystem.h"

AZombieSpawner::AZombieSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	bNetLoadOnClient = false;
}

void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();

	auto FindActorsSubsystem = GetWorld()->GetSubsystem<UFindActorsOfClassSubsystem>();
	if (FindActorsSubsystem)
	{
		TArray<AActor*> Zombies = FindActorsSubsystem->FindAllActors(AZombie::StaticClass());
		NumberOfZombiesPresent = Zombies.Num();

		FindActorsSubsystem->OnNewFindableActorRemoved.AddUObject(this, &AZombieSpawner::OnZombieDied);
		FindActorsSubsystem->OnNewFindableActorAdded.AddUObject(this, &AZombieSpawner::OnZombieSpawned);
	}

	for (TActorIterator<AZombieSpawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		SpawnLocations.Add(*ActorItr);
	}
}

void AZombieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnHordeTimeLeft -= DeltaTime;
	if (SpawnHordeTimeLeft < 0.f)
	{
		NumberOfZombiesToSpawn += 10;
		SpawnHordeTimeLeft = SpawnHordeTime;
	}

	SpawnSmallGroupTimeLeft -= DeltaTime;
	if (SpawnSmallGroupTimeLeft < 0.f)
	{
		NumberOfZombiesToSpawn += 2;
		SpawnSmallGroupTimeLeft = SpawnSmallGroupTime;
	}

	if (NumberOfZombiesToSpawn > 0)
	{
		for (AZombieSpawn* Spawn : SpawnLocations)
		{
			if (Spawn->CanSpawn())
			{
				const FVector SpawnLocation = Spawn->GetActorLocation();
				const FRotator SpawnRotation = Spawn->GetActorRotation();
				GetWorld()->SpawnActor(ZombieToSpawn, &SpawnLocation, &SpawnRotation);
				NumberOfZombiesToSpawn--;
				SpawnLocations.Swap(SpawnLocations.IndexOfByKey(Spawn), SpawnLocations.Num() - 1);
				break;
			}
		}
	}
}

void AZombieSpawner::OnZombieSpawned(UClass* Class, AActor* Actor)
{
	if (Class == AZombie::StaticClass())
	{
		NumberOfZombiesPresent++;
	}
}

void AZombieSpawner::OnZombieDied(UClass* Class, AActor* Actor)
{
	if (Class == AZombie::StaticClass())
	{
		NumberOfZombiesPresent--;
	}
}

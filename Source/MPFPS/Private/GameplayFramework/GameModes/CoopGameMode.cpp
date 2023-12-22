// Copyright Andrei Bondarenko 2023


#include "GameplayFramework/GameModes/CoopGameMode.h"
#include "ZombieSpawner/ZombieSpawner.h"

void ACoopGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->SpawnActor<AZombieSpawner>(*ZombieSpawner, FVector::ZeroVector, FRotator::ZeroRotator);
}

// Copyright Andrei Bondarenko 2023


#include "GameplayFramework/FPSGameMode.h"
#include "EngineUtils.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "GameplayFramework/FPSPlayerController.h"

static int32 PlayerIndex = 0;

APawn* AFPSGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	APawn* Pawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

	auto PlayerController = Cast<AFPSPlayerController>(NewPlayer);
	/*int32 Team = PlayerController->GetTeam();
	auto Character = Cast<APlayerCharacter>(Pawn);
	Character->SetTeam(Team);*/

	return Pawn;
}

AActor* AFPSGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PlayerStarts;
	for (TActorIterator<APlayerStart> It(GetWorld(), APlayerStart::StaticClass()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;
		PlayerStarts.Add(PlayerStart);
	}

	return PlayerStarts[PlayerIndex++ % PlayerStarts.Num()];
}

void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->SpawnActor<AActor>(*ActorToSpawn, FVector{1300.f, 1140.f, 70.f}, FRotator());
}

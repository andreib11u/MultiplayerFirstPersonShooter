// Copyright Andrei Bondarenko 2023


#include "GameplayFramework/FPSGameMode.h"
#include "EngineUtils.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "GameplayFramework/FPSPlayerController.h"
#include "Settings/FPSSettings.h"

static int32 PlayerIndex = 0;

APawn* AFPSGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	APawn* Pawn = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

	auto PlayerController = Cast<AFPSPlayerController>(NewPlayer);

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
}

void AFPSGameMode::StartPlay()
{
	Super::StartPlay();

	FGenericTeamId::SetAttitudeSolver(&UFPSSettings::GetAttitude);
}

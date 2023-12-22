// Copyright Andrei Bondarenko 2023


#include "GameplayFramework/FPSGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Settings/FPSSettings.h"

AActor* AFPSGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	static int32 PlayerIndex = 0;

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

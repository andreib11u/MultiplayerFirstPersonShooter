// Copyright Andrei Bondarenko 2023

#include "GameplayFramework/GameModes/DeathmatchGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

AActor* ADeathmatchGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PlayerStarts;
	static FName DeathmatchName = "Deathmatch";

	for (TActorIterator<APlayerStart> It(GetWorld(), APlayerStart::StaticClass()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;
		if (PlayerStart->PlayerStartTag == DeathmatchName)
		{
			PlayerStarts.Add(PlayerStart);
		}
	}

	if (PlayerStarts.IsEmpty())
	{
		return Super::ChoosePlayerStart(Player);
	}

	return PlayerStarts[FMath::RandRange(0, PlayerStarts.Num() - 1)];
}

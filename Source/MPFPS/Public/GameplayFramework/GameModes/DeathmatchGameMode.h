// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayFramework/FPSGameMode.h"
#include "DeathmatchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MPFPS_API ADeathmatchGameMode : public AFPSGameMode
{
	GENERATED_BODY()

	AActor* ChoosePlayerStart_Implementation(AController* Player);
};

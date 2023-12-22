// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayFramework/FPSGameMode.h"
#include "CoopGameMode.generated.h"

class AZombieSpawner;
/**
 * 
 */
UCLASS()
class MPFPS_API ACoopGameMode : public AFPSGameMode
{
	GENERATED_BODY()
public:

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AZombieSpawner> ZombieSpawner;
};

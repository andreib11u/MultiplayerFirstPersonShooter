// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPSGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerPawnSpawned, APawn*);

/**
 * 
 */
UCLASS()
class MPFPS_API AFPSGameMode : public AGameMode
{
	GENERATED_BODY()
protected:
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void BeginPlay() override;
	virtual void StartPlay() override;

};

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Types/FPSTypes.h"
#include "FPSGameInstance.generated.h"

class AGameMode;
/**
 * 
 */
UCLASS()
class MPFPS_API UFPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	TArray<FMapOptions> GetAvailableMaps() const { return AvailableMaps; }
	TMap<FName, TSoftClassPtr<AGameMode>> GetMapModes() const { return GameModes; }

private:
	UPROPERTY(EditAnywhere, Category = "Map Settings")
	TArray<FMapOptions> AvailableMaps;
	UPROPERTY(EditAnywhere, Category = "Map Settings")
	TMap<FName, TSoftClassPtr<AGameMode>> GameModes;

};

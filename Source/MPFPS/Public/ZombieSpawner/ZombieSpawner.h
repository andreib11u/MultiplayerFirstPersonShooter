// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawner.generated.h"

class AZombie;
class AZombieSpawn;

UCLASS()
class MPFPS_API AZombieSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	AZombieSpawner();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TArray<AZombieSpawn*> SpawnLocations;

	UPROPERTY(EditAnywhere)
	int32 NumberOfZombiesToSpawn = 10;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AZombie> ZombieToSpawn;

	UPROPERTY(VisibleAnywhere)
	int32 NumberOfZombiesPresent = 0;

	float TimeToSpawn = 5.f;

	UPROPERTY(EditAnywhere)
	float SpawnHordeTime = 15.f;
	UPROPERTY(EditAnywhere)
	float SpawnSmallGroupTime = 5.f;

	float SpawnSmallGroupTimeLeft = SpawnSmallGroupTime;
	float SpawnHordeTimeLeft = SpawnHordeTime;

	void OnZombieSpawned(UClass* Class, AActor* Actor);
	void OnZombieDied(UClass* Class, AActor* Actor);

};

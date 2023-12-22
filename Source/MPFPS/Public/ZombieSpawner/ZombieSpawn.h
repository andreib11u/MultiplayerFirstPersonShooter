// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawn.generated.h"

class UBoxComponent;
class AShootingCharacter;

UCLASS()
class MPFPS_API AZombieSpawn : public AActor
{
	GENERATED_BODY()
	
public:
	AZombieSpawn();

	bool DoesPlayerHaveLineOfSight() const { return bPlayerHasLineOfSight; }
	bool CanSpawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TArray<AShootingCharacter*> Players;

	UPROPERTY(EditAnywhere)
	USceneComponent* SpawnLocation;
	UPROPERTY(EditAnywhere)
	UBillboardComponent* BillboardComponent;
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;

	bool bPlayerHasLineOfSight;

	void OnNewPlayerAdded(UClass* Class, AActor* Actor);
	void OnNewPlayerRemoved(UClass* Class, AActor* Actor);

	void CheckPlayerLineOfSight();

};

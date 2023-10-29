// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FindActorsOfClassSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnNewFindableActorAdded, UClass*, AActor*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnNewFindableActorRemoved, UClass*, AActor*);

/**
 * 
 */
UCLASS()
class MPFPS_API UFindActorsOfClassSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	void AddActor(UClass* Type, AActor* Actor);
	void RemoveActor(UClass* Type, AActor* Actor);
	TArray<AActor*> FindAllActors(UClass* Type) const;

	FOnNewFindableActorAdded OnNewFindableActorAdded;
	FOnNewFindableActorRemoved OnNewFindableActorRemoved;

private:
	TMultiMap<UClass*, AActor*> SearchableActors;
};

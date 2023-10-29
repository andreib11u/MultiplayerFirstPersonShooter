// Copyright Andrei Bondarenko 2023


#include "Subsystems/FindActorsOfClassSubsystem.h"

void UFindActorsOfClassSubsystem::AddActor(UClass* Type, AActor* Actor)
{
	SearchableActors.AddUnique(Type, Actor);
	OnNewFindableActorAdded.Broadcast(Type, Actor);
}

void UFindActorsOfClassSubsystem::RemoveActor(UClass* Type, AActor* Actor)
{
	SearchableActors.RemoveSingle(Type, Actor);
	OnNewFindableActorRemoved.Broadcast(Type, Actor);
}

TArray<AActor*> UFindActorsOfClassSubsystem::FindAllActors(UClass* Type) const
{
	TArray<AActor*> FoundActors;
	SearchableActors.MultiFind(Type, FoundActors);

	return FoundActors;
}

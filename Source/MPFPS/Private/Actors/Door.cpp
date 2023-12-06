// Copyright Andrei Bondarenko 2023


#include "Actors/Door.h"
#include "Components/InteractionComponent.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("InteractionComponent");
	InteractionComponent->SetupAttachment(GetRootComponent());
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}


// Copyright Andrei Bondarenko 2023


#include "Components/InteractionComponent.h"
#include "Types/CollisionTypes.h"

void UInteractionComponent::PostInitProperties()
{
	Super::PostInitProperties();

	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(INTERACTION_TRACE_COLLISION, ECR_Block);
}

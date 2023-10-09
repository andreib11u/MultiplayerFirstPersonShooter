// Copyright Andrei Bondarenko 2023


#include "Animation/AnimNotifies/AnimNotify_GameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAnimNotify_GameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* MeshOwner = MeshComp->GetOwner();
	if (MeshOwner)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshOwner, EventTag, FGameplayEventData());
	}
}

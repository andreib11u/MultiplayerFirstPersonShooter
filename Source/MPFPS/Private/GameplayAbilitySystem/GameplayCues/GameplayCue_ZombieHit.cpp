// Copyright Andrei Bondarenko 2023


#include "GameplayAbilitySystem/GameplayCues/GameplayCue_ZombieHit.h"
#include "Kismet/GameplayStatics.h"

bool UGameplayCue_ZombieHit::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	AActor* EffectCauser = Parameters.GetEffectCauser();
	if (EffectCauser)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, EffectCauser->GetActorLocation(), EffectCauser->GetActorRotation());
	}

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

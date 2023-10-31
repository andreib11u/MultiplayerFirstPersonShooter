// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/GameplayCues/GameplayCue_ZombieHit.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/FPSHUD.h"
#include "UI/HUDWidget.h"

bool UGameplayCue_ZombieHit::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	AActor* EffectCauser = Parameters.GetEffectCauser();
	if (EffectCauser)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, EffectCauser->GetActorLocation(), EffectCauser->GetActorRotation(), 1, 1, 0,
											  SoundAttenuation);

		if (!MyTarget)
		{
			return true;
		}

		auto PlayerTarget = Cast<APlayerCharacter>(MyTarget);
		if (!PlayerTarget)
		{
			return true;
		}

		if (PlayerTarget->IsLocallyControlled())
		{
			auto PlayerController = PlayerTarget->GetController<APlayerController>();
			if (PlayerController)
			{
				PlayerController->GetHUD<AFPSHUD>()->GetHUDWidget()->AddDamageDirectionIndicator(EffectCauser);
			}
		}
	}

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

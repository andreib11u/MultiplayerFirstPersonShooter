// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/GameplayCues/GameplayCue_ZombieHit.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/FPSHUD.h"
#include "UI/HUDWidget.h"

void UGameplayCue_ZombieHit::AddDamageDirectionToPlayer(AActor* MyTarget, AActor* EffectCauser) const
{
	if (!MyTarget)
	{
		return;
	}

	auto PlayerTarget = Cast<APlayerCharacter>(MyTarget);
	if (!PlayerTarget)
	{
		return;
	}

	if (PlayerTarget->IsLocallyControlled())
	{
		auto PlayerController = PlayerTarget->GetController<APlayerController>();
		if (!PlayerController)
		{
			return;
		}

		auto HUD = PlayerController->GetHUD<AFPSHUD>();
		if (!HUD)
		{
			return;
		}

		UHUDWidget* HUDWidget = HUD->GetHUDWidget();
		if (!HUDWidget)
		{
			return;
		}

		HUDWidget->AddDamageDirectionIndicator(EffectCauser);
	}
}

bool UGameplayCue_ZombieHit::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	AActor* EffectCauser = Parameters.GetEffectCauser();
	if (EffectCauser)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, EffectCauser->GetActorLocation(), EffectCauser->GetActorRotation(), 1, 1, 0,
											  SoundAttenuation);

		AddDamageDirectionToPlayer(MyTarget, EffectCauser);
	}

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

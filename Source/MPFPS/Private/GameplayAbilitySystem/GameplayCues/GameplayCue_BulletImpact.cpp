// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/GameplayCues/GameplayCue_BulletImpact.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapons/WeaponComponent.h"

bool UGameplayCue_BulletImpact::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->ClientStartCameraShake(CameraShake);

	auto PlayerCharacter = Cast<APlayerCharacter>(Parameters.GetEffectCauser());
	if (PlayerCharacter)
	{
		check(PlayerCharacter->GetWeaponComponent()->GetCurrentWeapon());
		const FName SocketName = PlayerCharacter->GetWeaponComponent()->GetCurrentWeapon()->MuzzleSocket;

		USkeletalMeshComponent* WeaponMesh =
			PlayerCharacter->IsLocallyControlled() ? PlayerCharacter->GetFirstPersonWeaponMesh() : PlayerCharacter->GetThirdPersonWeaponMesh();

		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, WeaponMesh, PlayerCharacter->GetWeaponComponent()->GetCurrentWeapon()->MuzzleSocket);

		UParticleSystemComponent* SpawnedParticle =
			UGameplayStatics::SpawnEmitterAtLocation(PlayerCharacter->GetWorld(), ShotTrail, WeaponMesh->GetSocketLocation(SocketName));

		if (SpawnedParticle)
		{
			const FName ParticleParameterName = "ShockBeamEnd";
			SpawnedParticle->SetVectorParameter(ParticleParameterName, Parameters.Location);
		}

		UGameplayStatics::PlaySoundAtLocation(PlayerCharacter->GetWorld(), ShotSound, WeaponMesh->GetSocketLocation(SocketName), FRotator());

		if (Parameters.EffectContext.IsValid() && Parameters.EffectContext.GetHitResult()->GetActor())
		{
			UParticleSystem* ParticleSystem = nullptr;
			USoundBase* Sound;
			if (auto Character = Cast<APlayerCharacter>(Parameters.EffectContext.GetHitResult()->GetActor()))
			{
				if (GetWorld()->GetFirstPlayerController() != Character->GetController())
				{
					ParticleSystem = HumanImpactParticle;
				}
				Sound = HumanImpactSound;
			}
			else
			{
				ParticleSystem = ObjectImpactParticle;
				Sound = ObjectImpactSound;
			}

			UGameplayStatics::PlaySoundAtLocation(PlayerCharacter->GetWorld(), Sound, Parameters.Location, FRotator());
			UGameplayStatics::SpawnEmitterAtLocation(PlayerCharacter->GetWorld(), ParticleSystem, Parameters.Location,
													 Parameters.EffectContext.GetHitResult()->ImpactNormal.Rotation());
		}
	}

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
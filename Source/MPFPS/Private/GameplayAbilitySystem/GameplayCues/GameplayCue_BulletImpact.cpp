// Copyright Andrei Bondarenko 2023

#include "GameplayAbilitySystem/GameplayCues/GameplayCue_BulletImpact.h"
#include "Characters/ShootingCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapons/EquipmentComponent.h"

bool UGameplayCue_BulletImpact::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->ClientStartCameraShake(CameraShake);

	auto PlayerCharacter = Cast<AShootingCharacter>(Parameters.GetEffectCauser());
	if (PlayerCharacter)
	{
		auto Weapon = Cast<UWeapon>(PlayerCharacter->GetEquipmentComponent()->GetCurrentItem());
		check(Weapon);
		const FName SocketName = Weapon->GetMuzzleSocketName();

		USkeletalMeshComponent* WeaponMesh =
			PlayerCharacter->IsLocallyControlled() ? PlayerCharacter->GetFirstPersonWeaponMesh() : PlayerCharacter->GetThirdPersonWeaponMesh();

		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, WeaponMesh, Weapon->GetMuzzleSocketName());

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
			if (auto Character = Cast<ABaseCharacter>(Parameters.EffectContext.GetHitResult()->GetActor()))
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

			UGameplayStatics::PlaySoundAtLocation(PlayerCharacter->GetWorld(), Sound, Parameters.Location, PlayerCharacter->GetActorRotation(), 1, 1, 0, SoundAttenuation);
			UGameplayStatics::SpawnEmitterAtLocation(PlayerCharacter->GetWorld(), ParticleSystem, Parameters.Location,
													 Parameters.EffectContext.GetHitResult()->ImpactNormal.Rotation());
		}
	}

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

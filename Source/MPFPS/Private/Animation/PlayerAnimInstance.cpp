// Copyright Andrei Bondarenko 2023

#include "Animation/PlayerAnimInstance.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	auto PawnOwner = TryGetPawnOwner();
	if (!PawnOwner)
	{
		return;
	}

	PlayerCharacter = Cast<APlayerCharacter>(PawnOwner);
	check(PlayerCharacter);

	CharacterMovement = PlayerCharacter->GetCharacterMovement();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (PlayerCharacter)
	{
		const FVector Velocity = PlayerCharacter->GetVelocity();
		Speed = Velocity.Length();

		MovementDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, PlayerCharacter->GetActorRotation());

		BaseRotationPitch = PlayerCharacter->GetBaseAimRotation().Pitch;
		if (BaseRotationPitch > 180.f)
		{
			BaseRotationPitch -= 360.f;
		}
	}
}

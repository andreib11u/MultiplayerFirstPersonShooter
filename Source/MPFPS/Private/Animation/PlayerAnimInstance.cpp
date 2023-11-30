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

	if (!PlayerCharacter)
	{
		return;
	}

	const FVector Velocity = PlayerCharacter->GetVelocity();
	Speed = Velocity.Length();

	bIsFalling = CharacterMovement->IsFalling();

	MovementDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, PlayerCharacter->GetActorRotation());

	BaseRotationPitch = PlayerCharacter->GetBaseAimRotation().Pitch;
	if (BaseRotationPitch > 180.f)
	{
		BaseRotationPitch -= 360.f;
	}

	RightFootEffectorLocation = FVector{-PlayerCharacter->GetRightFootIKOffset(), 0.f, 0.f};
	LeftFootEffectorLocation = FVector{PlayerCharacter->GetLeftFootIKOffset(), 0.f, 0.f};
}

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class UAimOffsetBlendSpace1D;
class UCharacterMovementComponent;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class MPFPS_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly ,meta = (AllowPrivateAccess = "true"))
	UBlendSpace* MoveDirectionBlendspace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAimOffsetBlendSpace1D* AimOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MovementDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float BaseRotationPitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovement;
};

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MovementDirection;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float BaseRotationPitch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	FVector RightFootEffectorLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	FVector LeftFootEffectorLocation;


	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovement;
};

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Types/FPSGameplayAbilityTypes.h"
#include "FPSGameplayAbility.generated.h"

USTRUCT()
struct MPFPS_API FAbilityMeshMontage
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class USkeletalMeshComponent* Mesh;

	UPROPERTY()
	class UAnimMontage* Montage;

	FAbilityMeshMontage()
		: Mesh(nullptr)
		, Montage(nullptr)
	{
	}

	FAbilityMeshMontage(class USkeletalMeshComponent* InMesh, class UAnimMontage* InMontage)
		: Mesh(InMesh)
		, Montage(InMontage)
	{
	}
};

/**
 *
 */
UCLASS(Abstract)
class MPFPS_API UFPSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	void ReplicatedEndAbility();

	bool BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately);

	void SetCurrentMontageForMesh(USkeletalMeshComponent* InMesh, UAnimMontage* InCurrentMontage);
	bool FindAbilityMeshMontage(USkeletalMeshComponent* InMesh, FAbilityMeshMontage& InAbilityMeshMontage);


	EAbilityInput GetAbilityInput() const { return AbilityInput; }

protected:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UPROPERTY(EditAnywhere)
	EAbilityInput AbilityInput;

	/** Active montages being played by this ability */
	UPROPERTY()
	TArray<FAbilityMeshMontage> CurrentAbilityMeshMontages;

	UPROPERTY(EditAnywhere)
	bool bActivateWhenGranted = false;
};

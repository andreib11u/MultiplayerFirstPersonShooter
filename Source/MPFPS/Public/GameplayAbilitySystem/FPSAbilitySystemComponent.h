// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FPSAbilitySystemComponent.generated.h"

/**
 * Data about montages that were played locally (all montages in case of server. predictive montages in case of client). Never replicated directly.
 */
USTRUCT()
struct MPFPS_API FGameplayAbilityLocalAnimMontageForMesh
{
	GENERATED_BODY();

public:
	UPROPERTY()
	USkeletalMeshComponent* Mesh;

	UPROPERTY()
	FGameplayAbilityLocalAnimMontage LocalMontageInfo;

	FGameplayAbilityLocalAnimMontageForMesh()
		: Mesh(nullptr)
		, LocalMontageInfo()
	{
	}

	FGameplayAbilityLocalAnimMontageForMesh(USkeletalMeshComponent* InMesh)
		: Mesh(InMesh)
		, LocalMontageInfo()
	{
	}

	FGameplayAbilityLocalAnimMontageForMesh(USkeletalMeshComponent* InMesh, FGameplayAbilityLocalAnimMontage& InLocalMontageInfo)
		: Mesh(InMesh)
		, LocalMontageInfo(InLocalMontageInfo)
	{
	}
};

/**
 * Data about montages that is replicated to simulated clients.
 */
USTRUCT()
struct MPFPS_API FGameplayAbilityRepAnimMontageForMesh
{
	GENERATED_BODY();

public:
	UPROPERTY()
	USkeletalMeshComponent* Mesh;

	UPROPERTY()
	FGameplayAbilityRepAnimMontage RepMontageInfo;

	FGameplayAbilityRepAnimMontageForMesh()
		: Mesh(nullptr)
		, RepMontageInfo()
	{
	}

	FGameplayAbilityRepAnimMontageForMesh(USkeletalMeshComponent* InMesh)
		: Mesh(InMesh)
		, RepMontageInfo()
	{
	}
};

/**
 *
 */
UCLASS()
class MPFPS_API UFPSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	template <typename AttributeSet>
	const AttributeSet* GetAttributeSet(TSubclassOf<UAttributeSet> AttributeSetClass = AttributeSet::StaticClass());

	virtual bool ShouldDoServerAbilityRPCBatch() const override { return true; }
	bool BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool bEndAbilityImmediately);

	float PlayMontageForMesh(UGameplayAbility* InAnimatingAbility, USkeletalMeshComponent* InMesh, FGameplayAbilityActivationInfo ActivationInfo,
							 UAnimMontage* NewAnimMontage, float InPlayRate, FName StartSectionName, bool bReplicateMontage);

	float PlayMontageSimulatedForMesh(USkeletalMeshComponent* InMesh, UAnimMontage* NewAnimMontage, float InPlayRate, FName StartSectionName = NAME_None);

	// Stops whatever montage is currently playing. Expectation is caller should only be stopping it if they are the current animating ability (or have good reason not to check)
	virtual void CurrentMontageStopForMesh(USkeletalMeshComponent* InMesh, float OverrideBlendOutTime = -1.0f);

	// Returns the montage that is playing for the mesh
	UAnimMontage* GetCurrentMontageForMesh(USkeletalMeshComponent* InMesh);

	// Copy LocalAnimMontageInfo into RepAnimMontageInfo
	void AnimMontage_UpdateReplicatedDataForMesh(USkeletalMeshComponent* InMesh);
	void AnimMontage_UpdateReplicatedDataForMesh(FGameplayAbilityRepAnimMontageForMesh& OutRepAnimMontageInfo);

	// Called when a prediction key that played a montage is rejected
	void OnPredictiveMontageRejectedForMesh(USkeletalMeshComponent* InMesh, UAnimMontage* PredictiveMontage);

	// Returns the current animating ability
	UGameplayAbility* GetAnimatingAbilityFromAnyMesh();

	FGameplayAbilityLocalAnimMontageForMesh& GetLocalAnimMontageInfoForMesh(USkeletalMeshComponent* InMesh);
	FGameplayAbilityRepAnimMontageForMesh& GetGameplayAbilityRepAnimMontageForMesh(USkeletalMeshComponent* InMesh);

private:
	// Data structure for montages that were instigated locally (everything if server, predictive if client. replicated if simulated proxy)
	// Will be max one element per skeletal mesh on the AvatarActor
	UPROPERTY()
	TArray<FGameplayAbilityLocalAnimMontageForMesh> LocalAnimMontageInfoForMeshes;

	// Data structure for replicating montage info to simulated clients
	// Will be max one element per skeletal mesh on the AvatarActor
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedAnimMontageForMesh)
	TArray<FGameplayAbilityRepAnimMontageForMesh> RepAnimMontageInfoForMeshes;

	UFUNCTION()
	virtual void OnRep_ReplicatedAnimMontageForMesh();

	virtual bool IsReadyForReplicatedMontageForMesh();
};

template <typename AttributeSet>
const AttributeSet* UFPSAbilitySystemComponent::GetAttributeSet(TSubclassOf<UAttributeSet> AttributeSetClass)
{
	return Cast<AttributeSet>(UAbilitySystemComponent::GetAttributeSet(AttributeSetClass));
}

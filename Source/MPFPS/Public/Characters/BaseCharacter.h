// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Character.h"
#include "Types/FPSTypes.h"
#include "BaseCharacter.generated.h"

class UFPSGameplayAbility;
class UGameplayEffect;

UCLASS(Abstract)
class MPFPS_API ABaseCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void InitializeAttributes();

	TArray<FName> GetHeadBones() const { return HeadBones; }
	TArray<FName> GetLegBones() const { return LegBones; }

	virtual void GrantAbilities();

	float GetRightFootIKOffset() const { return CurrentRightFootOffset; }
	float GetLeftFootIKOffset() const { return CurrentLeftFootOffset; }

	// IGenericTeamAgentInterface begin

	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	// IGenericTeamAgentInterface end

	void SetLastDamageCauser(UAbilitySystemComponent* InLastDamageCauser);
	float GetReward() const { return RewardForKillingThis; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> AttributeInitializationEffect;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> OnDeathEffect;

	// attribute callbacks

	void OnWalkSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnCurrentHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData);

	virtual void OnZeroHealth();

	TWeakObjectPtr<UAbilitySystemComponent> LastDamageCauser;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> RewardEffect;
	UPROPERTY(EditAnywhere)
	float RewardForKillingThis = 50.f;

	bool bDissolve = false;

	UPROPERTY(EditAnywhere)
	TArray<FName> HeadBones;
	UPROPERTY(EditAnywhere)
	TArray<FName> LegBones;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UFPSGameplayAbility>> Abilities;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DeathEffect;

	UPROPERTY(EditAnywhere, Category = "Team", meta = (AllowPrivateAccess = "true"))
	EFPSTeam Team;

	float IKTraceDistance;
	float CurrentRightFootOffset;
	float CurrentLeftFootOffset;

	UPROPERTY(EditAnywhere, Category = "IK Setup")
	FName RightFootSocket;
	UPROPERTY(EditAnywhere, Category = "IK Setup")
	FName LeftFootSocket;
	UPROPERTY(EditAnywhere, Category = "IK Setup")
	float IKInterpSpeed = 20.f;

	float IKFootTrace(float TraceDistance, FName Bone);
};

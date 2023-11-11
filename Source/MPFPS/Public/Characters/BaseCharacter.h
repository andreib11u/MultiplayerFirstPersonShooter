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

	// IGenericTeamAgentInterface begin

	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	// IGenericTeamAgentInterface end

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> AttributeInitializationEffect;

	// attribute callbacks

	void OnWalkSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnCurrentHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData);

	virtual void Death();

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
};

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UFPSGameplayAbility;
class UGameplayEffect;

UCLASS(Abstract)
class MPFPS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void InitializeAttributes();

	TArray<FName> GetHeadBones() const { return HeadBones; }
	TArray<FName> GetLegBones() const { return LegBones; }

	virtual void GrantAbilities();

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
};

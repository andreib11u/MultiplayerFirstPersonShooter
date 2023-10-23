// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UGameplayEffect;

UCLASS(Abstract)
class MPFPS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void InitializeAttributes();

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
};

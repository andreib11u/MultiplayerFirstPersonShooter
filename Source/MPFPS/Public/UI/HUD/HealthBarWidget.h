// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "UI/FPSUserWidget.h"
#include "HealthBarWidget.generated.h"

class UFPSAbilitySystemComponent;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class MPFPS_API UHealthBarWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void Init(UFPSAbilitySystemComponent* AbilitySystemComponent);

	void SetCurrentHealth(const float CurrentHealth);
	void SetMaxHealth(float MaxHealth);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

	float CachedCurrentHealth = 0.f;
	float CachedMaxHealth = 1.f;

	void UpdateProgressBarPercent();
	void UpdateHealthBarText();

	void OnCurrentHealthChanged(const FOnAttributeChangeData& OnCurrentHealthChangeData);
	void OnMaxHealthChanged(const FOnAttributeChangeData& OnMaxHealthChangeData);
};

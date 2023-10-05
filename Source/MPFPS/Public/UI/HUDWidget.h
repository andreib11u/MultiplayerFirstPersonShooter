// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "EquippableItems/EquippableItem.h"
#include "UI/FPSUserWidget.h"
#include "HUDWidget.generated.h"

class UAmmoWidget;
class UHealthBarWidget;
class UFPSAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class MPFPS_API UHUDWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void Init(UFPSAbilitySystemComponent* AbilitySystemComponent);

private:
	UPROPERTY(meta = (BindWidget))
	UHealthBarWidget* HealthBarWidget;
	UPROPERTY(meta = (BindWidget))
	UAmmoWidget* AmmoWidget;

	UFUNCTION()
	void OnItemChanged(UEquippableItem* CurrentItem);
};

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UI/FPSUserWidget.h"
#include "HUDWidget.generated.h"

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
};

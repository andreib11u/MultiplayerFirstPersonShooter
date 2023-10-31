// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "EquippableItems/EquippableItem.h"
#include "UI/FPSUserWidget.h"
#include "HUDWidget.generated.h"

class APlayerCharacter;
class UCanvasPanel;
class UDamageDirectionIndicatorWidget;
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
	UFUNCTION(BlueprintCallable)
	void AddDamageDirectionIndicator(AActor* DamageCauser);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	UHealthBarWidget* HealthBarWidget;
	UPROPERTY(meta = (BindWidget))
	UAmmoWidget* AmmoWidget;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* HUDCanvas;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageDirectionIndicatorWidget> DamageDirectionIndicatorClass;
	UPROPERTY()
	TMap<AActor*, UDamageDirectionIndicatorWidget*> DamageDirectionIndicators;

	UFUNCTION()
	void OnItemChanged(UEquippableItem* CurrentItem);

	UPROPERTY()
	APlayerCharacter* CharacterOwner;

	void DisplayDamageDirectionIndicators();
};

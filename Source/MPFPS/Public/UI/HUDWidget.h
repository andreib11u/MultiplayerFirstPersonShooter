// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "EquippableItems/EquippableItem.h"
#include "UI/FPSUserWidget.h"
#include "HUDWidget.generated.h"

class UInteractionWidget;
class UMoneyWidget;
class UCrosshairWidget;
class AShootingCharacter;
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

	UInteractionWidget* GetInteractionWidget() const { return InteractionWidget; }

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	UHealthBarWidget* HealthBarWidget;
	UPROPERTY(meta = (BindWidget))
	UAmmoWidget* AmmoWidget;
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* HUDCanvas;
	UPROPERTY(meta = (BindWidget))
	UCrosshairWidget* Crosshair;
	UPROPERTY(meta = (BindWidget))
	UMoneyWidget* MoneyWidget;
	UPROPERTY(meta = (BindWidget))
	UInteractionWidget* InteractionWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageDirectionIndicatorWidget> DamageDirectionIndicatorClass;
	UPROPERTY()
	TMap<AActor*, UDamageDirectionIndicatorWidget*> DamageDirectionIndicators;

	UFUNCTION()
	void OnItemChanged(UEquippableItem* CurrentItem);

	UPROPERTY()
	AShootingCharacter* CharacterOwner;

	void DisplayDamageDirectionIndicators();
};

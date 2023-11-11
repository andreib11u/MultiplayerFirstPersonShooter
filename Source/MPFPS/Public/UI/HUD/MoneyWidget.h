// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "UI/FPSUserWidget.h"
#include "MoneyWidget.generated.h"

class UFPSAbilitySystemComponent;
class UMoneyAdditionWidget;
class UTextBlock;
class UOverlay;
/**
 * 
 */
UCLASS()
class MPFPS_API UMoneyWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void OnMoneyChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void Init(UFPSAbilitySystemComponent* AbilitySystemComponent);

	void SetMoney(float Money);
private:
	UPROPERTY(meta = (BindWidget))
	UOverlay* OverlayPanel;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoneyNumberText;

	int32 CurrentMoney;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMoneyAdditionWidget> MoneyAdditionWidgetClass;
	UPROPERTY()
	UMoneyAdditionWidget* MoneyAdditionWidget;

	UFUNCTION()
	void MoneyAdditionAnimationEnded(int32 MoneyToAdd);
};

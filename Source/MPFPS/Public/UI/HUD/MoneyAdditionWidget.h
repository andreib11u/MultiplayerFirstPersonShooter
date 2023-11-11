// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UI/FPSUserWidget.h"
#include "MoneyAdditionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyAnimationEnded, int32, MoneyToAdd);

class UTextBlock;
/**
 * 
 */
UCLASS()
class MPFPS_API UMoneyAdditionWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void Init(int32 Money);
	void AddMoney(int32 Money);

	UPROPERTY(BlueprintAssignable)
	FOnMoneyAnimationEnded OnMoneyAnimationEnded;

	bool IsAnimationStarted() const { return bAnimationStarted; }

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoneyText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* StartAnimation;

	UPROPERTY()
	UUMGSequencePlayer* SequencePlayer;

	UPROPERTY(EditAnywhere)
	float TimeFloating = 1.f;
	FTimerHandle FloatingTimerHandle;

	int32 CurrentMoney = 0;
	bool bAnimationStarted = false;

	void OnAdditionAnimationFinished(UUMGSequencePlayer& USequencePlayer);

	UFUNCTION()
	void OnFloatingTimerExpired();
};

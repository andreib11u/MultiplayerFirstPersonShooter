// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "UI/FPSUserWidget.h"
#include "CrosshairWidget.generated.h"

class UEquipmentComponent;
class UBorder;
class UFPSAbilitySystemComponent;
/**
 *
 */
UCLASS()
class MPFPS_API UCrosshairWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void Init(UEquipmentComponent* Equipment);
	void SetTargetSpread(float InSpread);
	void AddSpread(float InSpread);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	UBorder* Right;
	UPROPERTY(meta = (BindWidget))
	UBorder* Left;
	UPROPERTY(meta = (BindWidget))
	UBorder* Down;
	UPROPERTY(meta = (BindWidget))
	UBorder* Up;

	void CalculateCrosshairPosition();

	TInterval<float> RightPositions;
	TInterval<float> LeftPositions;
	TInterval<float> DownPositions;
	TInterval<float> UpPositions;

	UPROPERTY()
	UEquipmentComponent* EquipmentComponent;

	UPROPERTY(EditAnywhere)
	float SpreadAdjustmentSpeed = 3.f;

	float CurrentSpread = 0.1f;
	float TargetSpread = 0.1f;

	void OnSpreadAdded(float AddedSpread);
};

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Characters/ShootingCharacter.h"
#include "UI/FPSUserWidget.h"
#include "CrosshairWidget.generated.h"

class UImage;
class UEquipmentComponent;
class UBorder;
class UFPSAbilitySystemComponent;

DECLARE_STATS_GROUP(TEXT("FPSUI"), STATGROUP_FPSUI, STATCAT_Advanced);

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

	void ChangeColor(FLinearColor Color);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateColorWithTargetAttitude();
private:
	UPROPERTY(meta = (BindWidget))
	UBorder* Right;
	UPROPERTY(meta = (BindWidget))
	UBorder* Left;
	UPROPERTY(meta = (BindWidget))
	UBorder* Down;
	UPROPERTY(meta = (BindWidget))
	UBorder* Up;

	UPROPERTY(meta = (BindWidget))
	UImage* RightImage;
	UPROPERTY(meta = (BindWidget))
	UImage* LeftImage;
	UPROPERTY(meta = (BindWidget))
	UImage* DownImage;
	UPROPERTY(meta = (BindWidget))
	UImage* UpImage;

	UPROPERTY(EditAnywhere)
	FLinearColor ColorFriendly = FLinearColor::Green;
	UPROPERTY(EditAnywhere)
	FLinearColor ColorEnemy = FLinearColor::Red;
	UPROPERTY(EditAnywhere)
	FLinearColor OrdinaryCrosshairColor = FLinearColor::White;

	FLinearColor CurrentCrosshairColor;

	UPROPERTY()
	AShootingCharacter* PlayerCharacter;

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

	int32 BlockedTagCount = 0;

	void OnSpreadAdded(float AddedSpread);
	void OnBlockTagChanged(FGameplayTag GameplayTag, int32 Count);
};

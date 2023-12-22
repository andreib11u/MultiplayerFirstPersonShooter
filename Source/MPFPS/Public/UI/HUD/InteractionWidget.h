// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/InteractionComponent.h"
#include "UI/FPSUserWidget.h"
#include "InteractionWidget.generated.h"

class AShootingCharacter;
class UTextBlock;
class UProgressBar;
/**
 *
 */
UCLASS()
class MPFPS_API UInteractionWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void Init(AShootingCharacter* InPlayerCharacter);
	void ShowInteraction(const FText& InInteractionText);
	void HideInteraction();

	void SetInteractionProgress(float Percent);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* InteractionProgress;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RequiredInputText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionText;

	TWeakObjectPtr<AShootingCharacter> PlayerCharacter;
	TWeakObjectPtr<UInteractionComponent> InteractionComponent;

	UPROPERTY(EditAnywhere)
	float InteractionRange = 100.f;
};

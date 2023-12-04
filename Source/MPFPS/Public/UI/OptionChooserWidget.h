// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UI/FPSUserWidget.h"
#include "OptionChooserWidget.generated.h"

class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionIndexChanged, int32, OptionIndex);

/**
 *
 */
UCLASS()
class MPFPS_API UOptionChooserWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnOptionIndexChanged OnOptionIndexChanged;

	void SetOptionIndex(int32 Index, bool bBroadcast = true);

protected:
	virtual void NativeOnInitialized() override;

	virtual TArray<FText> GetOptions() const { return Options; }
private:
	UPROPERTY(meta = (BindWidget))
	UButton* LeftButton;
	UPROPERTY(meta = (BindWidget))
	UButton* RightButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* OptionText;

	int32 CurrentIndex = 0;

	UPROPERTY(EditAnywhere)
	TArray<FText> Options;

	UFUNCTION()
	void OnLeftButtonClicked();
	UFUNCTION()
	void OnRightButtonClicked();
};

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

class UHUDWidget;

/**
 * 
 */
UCLASS()
class MPFPS_API AFPSHUD : public AHUD
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CreateHUD();
	UFUNCTION(BlueprintCallable)
	void DestroyHUD();


	UHUDWidget* GetHUDWidget() const { return HUDWidget; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UPROPERTY()
	UHUDWidget* HUDWidget;
};

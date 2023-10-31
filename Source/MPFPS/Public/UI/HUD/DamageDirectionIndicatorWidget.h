// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UI/FPSUserWidget.h"
#include "DamageDirectionIndicatorWidget.generated.h"

/**
 * 
 */
UCLASS()
class MPFPS_API UDamageDirectionIndicatorWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void CalculateTransform();
	void Init(AActor* InOriginActor, AActor* InDamageCauser);
	void RemoveAndDeactivate();
	void ResetTimer();

	bool IsActive() const { return bActive; }

private:
	UPROPERTY(EditAnywhere)
	float DisappearTime = 1.5f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float DistanceFromCenter = 0.3f;

	TWeakObjectPtr<AActor> DamageCauser;
	TWeakObjectPtr<AActor> OriginActor;

	FTimerHandle DisappearTimerHandle;

	bool bActive = true;
};

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "FPSGameState.generated.h"

class UFPSSessionSettings;

/**
 * 
 */
UCLASS()
class MPFPS_API AFPSGameState : public AGameState
{
	GENERATED_BODY()
public:
	UFPSSessionSettings* GetSessionSettings() const { return SessionSettings; }

protected:
	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(EditAnywhere, Category = "Settings", ReplicatedUsing=OnRep_SessionSettingsClass)
	TSubclassOf<UFPSSessionSettings> SessionSettingsClass;
	UPROPERTY()
	UFPSSessionSettings* SessionSettings;

	UFUNCTION()
	void OnRep_SessionSettingsClass();
};

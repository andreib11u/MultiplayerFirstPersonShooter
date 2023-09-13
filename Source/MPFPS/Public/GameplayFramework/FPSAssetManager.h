// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "FPSAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class MPFPS_API UFPSAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	virtual void StartInitialLoading() override;
};

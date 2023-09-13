// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputDataAsset.generated.h"

class UInputAction;

/**
 * 
 */
UCLASS()
class MPFPS_API UInputDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	UInputAction* Move;
	UPROPERTY(EditAnywhere)
	UInputAction* Look;
	UPROPERTY(EditAnywhere)
	UInputAction* Jump;
	UPROPERTY(EditAnywhere)
	UInputAction* PrimaryAction;
	UPROPERTY(EditAnywhere)
	UInputAction* SecondaryAction;
	UPROPERTY(EditAnywhere)
	UInputAction* Use;

	UPROPERTY(EditAnywhere)
	UInputAction* Ability1;
	UPROPERTY(EditAnywhere)
	UInputAction* Ability2;

};

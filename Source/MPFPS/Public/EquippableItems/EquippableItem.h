// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EquippableItem.generated.h"

class UEquipmentComponent;
class UGameplayAbility;
/**
 *
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class MPFPS_API UEquippableItem : public UObject
{
	GENERATED_BODY()
public:
	virtual void OnEquip(UEquipmentComponent* EquipmentComponent);

	TSubclassOf<UGameplayAbility> GetAbility() const { return UseAbility; }
	USkeletalMesh* GetItemMesh() const { return EquipMesh; }

private:
	UPROPERTY(EditAnywhere)
	USkeletalMesh* EquipMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> UseAbility;

public:
	UPROPERTY(EditAnywhere)
	FVector ThirdPersonLocation;
	UPROPERTY(EditAnywhere)
	FRotator ThirdPersonRotation;
	UPROPERTY(EditAnywhere)
	FVector FirstPersonLocation;
	UPROPERTY(EditAnywhere)
	FRotator FirstPersonRotation;

	UPROPERTY(EditAnywhere)
	FVector ArmsMeshRelativeLocation;
	UPROPERTY(EditAnywhere)
	FVector ArmsMeshRelativeLocationWhenAiming;
};

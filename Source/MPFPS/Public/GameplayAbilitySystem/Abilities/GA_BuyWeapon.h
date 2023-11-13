// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GA_GiveWeapon.h"
#include "GameplayAbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "GA_BuyWeapon.generated.h"

class UWeapon;
/**
 *
 */
UCLASS()
class MPFPS_API UGA_BuyWeapon : public UGA_GiveWeapon
{
	GENERATED_BODY()
public:
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
						   FGameplayTagContainer* OptionalRelevantTags) const override;

private:
	UPROPERTY(EditAnywhere)
	float WeaponCost = 500.f;
};

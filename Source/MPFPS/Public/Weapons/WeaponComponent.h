// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentWeaponChanged, UWeapon*, CurrentWeapon);


class UWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MPFPS_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

	UPROPERTY(BlueprintAssignable)
	FOnCurrentWeaponChanged OnCurrentWeaponChanged;

	UFUNCTION(BlueprintPure)
	bool HasWeapon() const { return CurrentWeapon != nullptr; }

	void SetCurrentWeapon(const int32 WeaponIndex);
	void SetCurrentWeapon(UWeapon* Weapon);
	UWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
	void AddWeapon(UWeapon* Weapon);
	void AddWeapon(TSubclassOf<UWeapon> WeaponClass);

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	UWeapon* CurrentWeapon;

	UPROPERTY(Replicated)
	TSubclassOf<UWeapon> CurrentWeaponClass;

	UPROPERTY(EditAnywhere, Replicated)
	TArray<UWeapon*> Weapons;
	int32 CurrentWeaponIndex = 0;

};

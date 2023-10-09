// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

class UAbilitySystemComponent;
class UEquippableItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentItemChanged, UEquippableItem*, CurrentItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentClipAmmoChanged, float, CurrentClipAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentReserveAmmoChanged, float, CurrentReserveAmmo);

class UWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MPFPS_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentComponent();

	UPROPERTY(BlueprintAssignable)
	FOnCurrentItemChanged OnCurrentItemChanged;

	UFUNCTION(BlueprintPure)
	bool HasItemInHands() const { return CurrentItem != nullptr; }

	void EquipItem(const int32 ItemIndex);
	void EquipItem(UEquippableItem* Item);
	UEquippableItem* GetCurrentItem() const { return CurrentItem; }

	void AddWeapon(TSubclassOf<UEquippableItem> ItemClass);

	UPROPERTY(BlueprintAssignable)
	FOnCurrentClipAmmoChanged OnCurrentClipAmmoChanged;
	UPROPERTY(BlueprintAssignable)
	FOnCurrentReserveAmmoChanged OnCurrentReserveAmmoChanged;

	void SpendAmmo(float Ammo);
	void ReloadAmmo();

	void SetCurrentClipAmmo(float Ammo);
	void SetCurrentReserveAmmo(float Ammo);
	float GetCurrentClipAmmo() const { return CurrentClipAmmo; }
	float GetCurrentReserveAmmo() const { return CurrentReserveAmmo; }
	bool IsMaxClipAmmo() const;

	void SetAmmoFrom(UWeapon* Weapon);

	void SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent) { AbilitySystemComponent = InAbilitySystemComponent; }
	UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;

private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	UEquippableItem* CurrentItem;

	UPROPERTY(Replicated)
	TSubclassOf<UEquippableItem> CurrentItemClass;

	UPROPERTY(EditAnywhere, Replicated)
	TArray<UEquippableItem*> Items;
	int32 CurrentItemIndex = 0;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_CurrentClipAmmo)
	float CurrentClipAmmo;
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_CurrentReserveAmmo)
	float CurrentReserveAmmo;

	UPROPERTY(EditAnywhere)
	float MaxClipAmmo;
	UPROPERTY(EditAnywhere)
	float MaxReserveAmmo;

	UPROPERTY(EditAnywhere)
	FGameplayTag WeaponIsFiringTag = FGameplayTag::RequestGameplayTag("Weapon.IsFiring");

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UFUNCTION()
	void OnRep_CurrentClipAmmo();
	UFUNCTION()
	void OnRep_CurrentReserveAmmo();
};

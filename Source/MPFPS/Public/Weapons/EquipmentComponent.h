// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

class UEquippableItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentItemChanged, UEquippableItem*, CurrentItem);


class UWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MPFPS_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentComponent();

	UPROPERTY(BlueprintAssignable)
	FOnCurrentItemChanged OnCurrentItemChanged;

	UFUNCTION(BlueprintPure)
	bool HasItemInHands() const { return CurrentItem != nullptr; }

	void SetCurrentItem(const int32 ItemIndex);
	void SetCurrentItem(UEquippableItem* Item);
	UEquippableItem* GetCurrentItem() const { return CurrentItem; }

	void AddWeapon(TSubclassOf<UEquippableItem> ItemClass);

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleInstanceOnly, Replicated)
	UEquippableItem* CurrentItem;

	UPROPERTY(Replicated)
	TSubclassOf<UEquippableItem> CurrentItemClass;

	UPROPERTY(EditAnywhere, Replicated)
	TArray<UEquippableItem*> Items;
	int32 CurrentItemIndex = 0;

};

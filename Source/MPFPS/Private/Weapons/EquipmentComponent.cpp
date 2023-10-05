// Copyright Andrei Bondarenko 2023


#include "Weapons/EquipmentComponent.h"

#include "AbilitySystemInterface.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Types/FPSGameplayAbilityTypes.h"
#include "Weapons/Weapon.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipmentComponent, CurrentItem);
	DOREPLIFETIME_CONDITION(UEquipmentComponent, Items, COND_OwnerOnly);
	DOREPLIFETIME(UEquipmentComponent, CurrentItemClass);
}

void UEquipmentComponent::SetCurrentItem(const int32 ItemIndex)
{
	if (Items.IsValidIndex(ItemIndex))
	{
		SetCurrentItem(Items[ItemIndex]);
	}
}

void UEquipmentComponent::SetCurrentItem(UEquippableItem* Item)
{
	if (CurrentItem)
	{
		RemoveReplicatedSubObject(CurrentItem);
	}

	CurrentItem = Item;

	if (CurrentItem)
	{
		CurrentItemClass = CurrentItem->GetClass();
		if (auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(GetOwner()))
		{
			if (auto AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
			{
				FGameplayAbilitySpec Spec = FGameplayAbilitySpec(CurrentItem->GetAbility(), 1, static_cast<int32>(EAbilityInput::PrimaryAction), GetOwner());
				AbilitySystemComponent->GiveAbility(Spec);
			}
		}
	}

	AddReplicatedSubObject(CurrentItem);
	OnCurrentItemChanged.Broadcast(CurrentItem);
}

void UEquipmentComponent::AddWeapon(TSubclassOf<UEquippableItem> ItemClass)
{
	auto Item = NewObject<UEquippableItem>(this, *ItemClass);
	const int32 Index = Items.Add(Item);
	SetCurrentItem(Index);
}

void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CurrentItemClass)
	{
		CurrentItem = NewObject<UEquippableItem>(this, CurrentItemClass);
	}

	OnCurrentItemChanged.Broadcast(CurrentItem);
}


// Copyright Andrei Bondarenko 2023


#include "Weapons/WeaponComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/Weapon.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponComponent, CurrentWeapon);
	DOREPLIFETIME_CONDITION(UWeaponComponent, Weapons, COND_OwnerOnly);
}

void UWeaponComponent::SetCurrentWeapon(const int32 WeaponIndex)
{
	if (Weapons.IsValidIndex(WeaponIndex))
	{
		SetCurrentWeapon(Weapons[WeaponIndex]);
	}
}

void UWeaponComponent::SetCurrentWeapon(UWeapon* Weapon)
{
	if (CurrentWeapon)
	{
		RemoveReplicatedSubObject(CurrentWeapon);
	}
	CurrentWeapon = Weapon;
	AddReplicatedSubObject(CurrentWeapon);
	OnCurrentWeaponChanged.Broadcast(CurrentWeapon);
}

void UWeaponComponent::AddWeapon(UWeapon* Weapon)
{
	const int32 Index = Weapons.Add(Weapon);
	SetCurrentWeapon(Index);
}

void UWeaponComponent::AddWeapon(TSubclassOf<UWeapon> WeaponClass)
{
	auto Weapon = NewObject<UWeapon>(this, *WeaponClass);
	const int32 Index = Weapons.Add(Weapon);
	SetCurrentWeapon(Index);
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


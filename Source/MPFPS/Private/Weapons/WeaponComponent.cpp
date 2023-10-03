// Copyright Andrei Bondarenko 2023


#include "Weapons/WeaponComponent.h"

#include "AbilitySystemInterface.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Types/FPSGameplayAbilityTypes.h"
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
	DOREPLIFETIME(UWeaponComponent, CurrentWeaponClass);
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

	if (CurrentWeapon)
	{
		CurrentWeaponClass = CurrentWeapon->GetClass();
		if (auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(GetOwner()))
		{
			if (auto AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
			{
				FGameplayAbilitySpec Spec = FGameplayAbilitySpec(CurrentWeapon->ShootAbility, 1, static_cast<int32>(EAbilityInput::PrimaryAction), GetOwner());
				AbilitySystemComponent->GiveAbility(Spec);
			}
		}
	}

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

	/*if (CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ammo: %f"), CurrentWeapon->CurrentClipAmmo)
	}*/
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CurrentWeaponClass)
	{
		CurrentWeapon = NewObject<UWeapon>(this, CurrentWeaponClass);
	}

	OnCurrentWeaponChanged.Broadcast(CurrentWeapon);
}


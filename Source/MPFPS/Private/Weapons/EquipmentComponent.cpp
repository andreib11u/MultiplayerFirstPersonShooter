// Copyright Andrei Bondarenko 2023

#include "Weapons/EquipmentComponent.h"

#include "Characters/ShootingCharacter.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Types/FPSGameplayAbilityTypes.h"
#include "Weapons/Weapon.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UEquipmentComponent, Items, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UEquipmentComponent, CurrentClipAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UEquipmentComponent, CurrentReserveAmmo, COND_OwnerOnly);
	DOREPLIFETIME(UEquipmentComponent, CurrentItemClass);
}

void UEquipmentComponent::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	DOREPLIFETIME_ACTIVE_OVERRIDE(UEquipmentComponent, CurrentClipAmmo,
								  (IsValid(AbilitySystemComponent) && !AbilitySystemComponent->HasMatchingGameplayTag(WeaponIsFiringTag)));
	DOREPLIFETIME_ACTIVE_OVERRIDE(UEquipmentComponent, CurrentReserveAmmo,
								  (IsValid(AbilitySystemComponent) && !AbilitySystemComponent->HasMatchingGameplayTag(WeaponIsFiringTag)));
}

void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PlayerCharacterOwner->GetAbilitySystemComponent())
	{
		return;
	}

	if (PlayerCharacterOwner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Weapon.State.Aiming")))
	{
		TargetSpread = CurrentWeaponStats.AimSpread;
	}
	else
	{
		const double OwnerSpeedSquared = PlayerCharacterOwner->GetVelocity().SquaredLength();

		TargetSpread = OwnerSpeedSquared > 0.f ? CurrentWeaponStats.WalkSpread : CurrentWeaponStats.StandSpread;
		TargetMaxSpread = OwnerSpeedSquared > 0.f ? CurrentWeaponStats.MaxWalkSpread : CurrentWeaponStats.MaxStandSpread;
	}
	
	Spread = TargetSpread + AddedSpread;

	AddedSpread = FMath::FInterpConstantTo(AddedSpread, 0.f, DeltaTime, CurrentWeaponStats.SpreadDecay);
}

void UEquipmentComponent::OnRep_CurrentItemClass()
{
	if (CurrentItemClass)
	{
		AddWeapon(CurrentItemClass);
	}
}

void UEquipmentComponent::OnRep_CurrentClipAmmo()
{
	OnCurrentClipAmmoChanged.Broadcast(CurrentClipAmmo);
}

void UEquipmentComponent::OnRep_CurrentReserveAmmo()
{
	OnCurrentReserveAmmoChanged.Broadcast(CurrentReserveAmmo);
}

void UEquipmentComponent::EquipItem(const int32 ItemIndex)
{
	if (Items.IsValidIndex(ItemIndex))
	{
		EquipItem(Items[ItemIndex]);
	}
}

void UEquipmentComponent::EquipItem(UEquippableItem* Item)
{
	if (CurrentItem)
	{
		if (GetOwner()->HasAuthority())
		{
			if (AbilitySystemComponent)
			{
				AbilitySystemComponent->ClearAbility(CurrentWeaponAbilityHandle);
			}
		}
	}

	CurrentItem = Item;

	if (CurrentItem)
	{
		CurrentItem->OnEquip(this);
		CurrentItemClass = CurrentItem->GetClass();

		if (GetOwner()->HasAuthority())
		{
			if (AbilitySystemComponent)
			{
				FGameplayAbilitySpec Spec =
					FGameplayAbilitySpec(CurrentItem->GetAbility(), 1, static_cast<int32>(EAbilityInput::PrimaryAction), GetOwner());
				CurrentWeaponAbilityHandle = AbilitySystemComponent->GiveAbility(Spec);
			}
		}
	}

	OnCurrentItemChanged.Broadcast(CurrentItem);
}

UWeapon* UEquipmentComponent::GetCurrentWeapon() const
{
	return Cast<UWeapon>(CurrentItem);
}

void UEquipmentComponent::AddWeapon(TSubclassOf<UEquippableItem> ItemClass)
{
	auto Item = NewObject<UEquippableItem>(this, *ItemClass);
	const int32 Index = Items.Add(Item);
	EquipItem(Index);
}

bool UEquipmentComponent::IsMaxClipAmmo() const
{
	return CurrentClipAmmo == CurrentWeaponStats.MaxClipAmmo;
}

void UEquipmentComponent::SetAmmoFrom(UWeapon* Weapon)
{
	CurrentWeaponStats = Weapon->WeaponStats;
	CurrentClipAmmo = CurrentWeaponStats.MaxClipAmmo;
	CurrentReserveAmmo = CurrentWeaponStats.MaxReserveAmmo;

	OnCurrentReserveAmmoChanged.Broadcast(CurrentReserveAmmo);
	OnCurrentClipAmmoChanged.Broadcast(CurrentClipAmmo);
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CurrentItemClass)
	{
		CurrentItem = NewObject<UEquippableItem>(this, CurrentItemClass);
	}

	OnCurrentItemChanged.Broadcast(CurrentItem);

	PlayerCharacterOwner = Cast<AShootingCharacter>(GetOwner());
	checkf(PlayerCharacterOwner, TEXT("EquipmentComponent attached not to a AShootingCharacter"))

		OwnerMovement = PlayerCharacterOwner->GetCharacterMovement();
}

void UEquipmentComponent::SpendAmmo(float Ammo)
{
	check(CurrentClipAmmo >= Ammo);

	CurrentClipAmmo -= Ammo;
	OnCurrentClipAmmoChanged.Broadcast(CurrentClipAmmo);
}

void UEquipmentComponent::ReloadAmmo()
{
	const bool bHasReserveAmmo = CurrentReserveAmmo > 0.f;
	const bool bNeedsReload = CurrentClipAmmo < CurrentWeaponStats.MaxClipAmmo;

	if (bHasReserveAmmo && bNeedsReload)
	{
		const float AmmoNeededToFull = CurrentWeaponStats.MaxClipAmmo - CurrentClipAmmo;
		if (AmmoNeededToFull >= CurrentReserveAmmo)
		{
			CurrentClipAmmo += CurrentReserveAmmo;
			CurrentReserveAmmo = 0.f;
		}
		else
		{
			CurrentClipAmmo += AmmoNeededToFull;
			CurrentReserveAmmo -= AmmoNeededToFull;
		}

		OnCurrentClipAmmoChanged.Broadcast(CurrentClipAmmo);
		OnCurrentReserveAmmoChanged.Broadcast(CurrentReserveAmmo);
	}
}

void UEquipmentComponent::AddSpread()
{
	if (!PlayerCharacterOwner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Weapon.State.Aiming")))
	{
		AddedSpread = FMath::Clamp(AddedSpread + GetWeaponStats().SpreadPerShot, 0.f, TargetMaxSpread);
		OnSpreadAdded.ExecuteIfBound(GetWeaponStats().SpreadPerShot);
	}
}

void UEquipmentComponent::SetCurrentClipAmmo(float Ammo)
{
	CurrentClipAmmo = Ammo;
	OnCurrentClipAmmoChanged.Broadcast(CurrentClipAmmo);
}

void UEquipmentComponent::SetCurrentReserveAmmo(float Ammo)
{
	CurrentReserveAmmo = Ammo;
	OnCurrentReserveAmmoChanged.Broadcast(CurrentReserveAmmo);
}

// Copyright Andrei Bondarenko 2023


#include "Weapons/EquipmentComponent.h"

#include "Characters/PlayerCharacter.h"
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

	const double OwnerSpeedSquared = PlayerCharacterOwner->GetVelocity().SquaredLength();

	TargetSpread = OwnerSpeedSquared > 0.f ? CurrentWeaponStats.WalkSpread : CurrentWeaponStats.StandSpread;
	Spread = TargetSpread + AddedSpread;

	AddedSpread = FMath::FInterpConstantTo(AddedSpread, 0.f, DeltaTime, CurrentWeaponStats.SpreadDecay);
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
				AbilitySystemComponent->GiveAbility(Spec);
			}
		}
	}

	OnCurrentItemChanged.Broadcast(CurrentItem);
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

	PlayerCharacterOwner = Cast<APlayerCharacter>(GetOwner());
	checkf(PlayerCharacterOwner, TEXT("EquipmentComponent attached not to a APlayerCharacter"))

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

void UEquipmentComponent::AddSpread(float InSpread)
{
	AddedSpread += InSpread;
	OnSpreadAdded.ExecuteIfBound(InSpread);
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


// Copyright Andrei Bondarenko 2023

#include "Characters/PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DemoNetDriver.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "GameplayAbilitySystem/Abilities/GameplayAbility_FireOnce.h"
#include "GameplayAbilitySystem/Abilities/PrintTextGameplayAbility.h"
#include "GameplayFramework/FPSPlayerState.h"
#include "Input/InputDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "Types/FPSGameplayAbilityTypes.h"
#include "Weapons/Weapon.h"
#include "Weapons/EquipmentComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerCharacter, All, All);

APlayerCharacter::APlayerCharacter()
{
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("FirstPersonCamera");
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->SetRelativeLocation(FVector{-10.f, 0.f, 60.f});
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonMesh");
	FirstPersonMesh->SetRelativeLocation(FVector{-30.f, 0.f, -151.32f});
	FirstPersonMesh->SetCollisionProfileName("CharacterMesh");
	FirstPersonMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
	FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>("EquipmentComponent");
	EquipmentComponent->OnCurrentItemChanged.AddDynamic(this, &APlayerCharacter::OnItemChanged);

	FirstPersonWeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("FPWeaponMesh");
	FirstPersonWeaponMeshComponent->SetupAttachment(FirstPersonMesh, "WeaponPoint");

	ThirdPersonWeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("TPWeaponMesh");
	ThirdPersonWeaponMeshComponent->SetupAttachment(GetMesh(), "weapon_r");
}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMappingContext, 0);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Bind the actions
	if (!InputActions || !InputMappingContext)
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("InputActions and/or InputMappingContext is null. Character input isn't setup"))
		return;
	}

	if (InputActions->Look)
	{
		EnhancedInputComponent->BindAction(InputActions->Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	}

	if (InputActions->Move)
	{
		EnhancedInputComponent->BindAction(InputActions->Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	}

	if (InputActions->Jump)
	{
		EnhancedInputComponent->BindAction(InputActions->Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(InputActions->Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}

	if (InputActions->PrimaryAction)
	{
		EnhancedInputComponent->BindAction(InputActions->PrimaryAction, ETriggerEvent::Started, this, &APlayerCharacter::PrimaryActionPressed);
		EnhancedInputComponent->BindAction(InputActions->PrimaryAction, ETriggerEvent::Completed, this, &APlayerCharacter::PrimaryActionReleased);
	}

	if (InputActions->SecondaryAction)
	{
		EnhancedInputComponent->BindAction(InputActions->SecondaryAction, ETriggerEvent::Started, this,
										   &APlayerCharacter::SecondaryActionActionPressed);
		EnhancedInputComponent->BindAction(InputActions->SecondaryAction, ETriggerEvent::Completed, this,
										   &APlayerCharacter::SecondaryActionActionReleased);
	}

	if (InputActions->Reload)
	{
		EnhancedInputComponent->BindAction(InputActions->Reload, ETriggerEvent::Started, this, &APlayerCharacter::ReloadPressed);
		EnhancedInputComponent->BindAction(InputActions->Reload, ETriggerEvent::Completed, this, &APlayerCharacter::ReloadReleased);
	}
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AFPSPlayerState* FPSPlayerState = GetPlayerState<AFPSPlayerState>();
	if (!ensure(FPSPlayerState))
	{
		return;
	}

	// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
	AbilitySystemComponent = Cast<UFPSAbilitySystemComponent>(FPSPlayerState->GetAbilitySystemComponent());
	check(AbilitySystemComponent);

	// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
	AbilitySystemComponent->InitAbilityActorInfo(FPSPlayerState, this);

	EquipmentComponent->SetAbilitySystemComponent(AbilitySystemComponent);

	GrantAbilities();

	InitializeAttributes();
}

// void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(APlayerCharacter, CharacterAmmo);
// }

void APlayerCharacter::GrantAbilities()
{
	for (TSubclassOf<UFPSGameplayAbility> Ability : Abilities)
	{
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(Ability, 1, 0, this);
		if (ensure(AbilitySystemComponent))
		{
			AbilitySystemComponent->GiveAbility(Spec);
		}
	}
}

void APlayerCharacter::InitializeAttributes()
{
	if (!AttributeInitializationEffect)
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("%s missing AttributeInitializationEffect."), *GetName())
		return;
	}

	check(AbilitySystemComponent);

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(AttributeInitializationEffect, 1.f, EffectContext);
	if (EffectSpec.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
	}
}

void APlayerCharacter::OnItemChanged(UEquippableItem* Item)
{
	if (Item)
	{
		if (IsLocallyControlled())
		{
			FirstPersonWeaponMeshComponent->SetVisibility(true, true);
			FirstPersonWeaponMeshComponent->SetSkeletalMesh(Item->GetItemMesh());
			FirstPersonWeaponMeshComponent->SetRelativeLocation(Item->FirstPersonLocation);
			FirstPersonWeaponMeshComponent->SetRelativeRotation(Item->FirstPersonRotation);
		}
		else
		{
			ThirdPersonWeaponMeshComponent->SetVisibility(true, true);
			ThirdPersonWeaponMeshComponent->SetSkeletalMesh(Item->GetItemMesh());
			ThirdPersonWeaponMeshComponent->SetRelativeLocation(Item->ThirdPersonLocation);
			ThirdPersonWeaponMeshComponent->SetRelativeRotation(Item->ThirdPersonRotation);
		}
	}
	else
	{
		FirstPersonWeaponMeshComponent->SetVisibility(false, true);
		ThirdPersonWeaponMeshComponent->SetVisibility(false, true);
	}
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AFPSPlayerState* FPSPlayerState = GetPlayerState<AFPSPlayerState>();
	if (!ensure(FPSPlayerState))
	{
		return;
	}

	// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
	AbilitySystemComponent = Cast<UFPSAbilitySystemComponent>(FPSPlayerState->GetAbilitySystemComponent());
	check(AbilitySystemComponent);

	// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
	AbilitySystemComponent->InitAbilityActorInfo(FPSPlayerState, this);

	EquipmentComponent->SetAbilitySystemComponent(AbilitySystemComponent);

	InitializeAttributes();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		GetMesh()->SetVisibility(false, true);
		FirstPersonMesh->SetVisibility(true, true);
	}
	else
	{
		GetMesh()->SetVisibility(true, true);
		FirstPersonMesh->SetVisibility(false, true);
	}
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APlayerCharacter::Look(const FInputActionValue& InputActionValue)
{
	const auto Value = InputActionValue.Get<FVector2D>();

	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void APlayerCharacter::Move(const FInputActionValue& InputActionValue)
{
	const auto Value = InputActionValue.Get<FVector2D>();

	AddMovementInput(GetActorRightVector(), Value.X);
	AddMovementInput(GetActorForwardVector(), Value.Y);
}

void APlayerCharacter::PrimaryActionPressed()
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EAbilityInput::PrimaryAction));
}

void APlayerCharacter::PrimaryActionReleased()
{
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(EAbilityInput::PrimaryAction));
}

void APlayerCharacter::SecondaryActionActionPressed()
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EAbilityInput::SecondaryAction));
}

void APlayerCharacter::SecondaryActionActionReleased()
{
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(EAbilityInput::SecondaryAction));
}

void APlayerCharacter::ReloadPressed()
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EAbilityInput::Reload));
}

void APlayerCharacter::ReloadReleased()
{
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(EAbilityInput::Reload));
}

// Copyright Andrei Bondarenko 2023

#include "Characters/PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "GameplayAbilitySystem/Abilities/PrintTextGameplayAbility.h"
#include "GameplayFramework/FPSPlayerState.h"
#include "Input/InputDataAsset.h"
#include "Types/FPSGameplayAbilityTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerCharacter, All, All);

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
		UE_LOG(LogTemp, Error, TEXT("InputActions and/or InputMappingContext is null. Character input isn't setup"))
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

	GrantAbilities();
}

void APlayerCharacter::GrantAbilities()
{
	FGameplayAbilitySpec Spec =
		FGameplayAbilitySpec(UPrintTextGameplayAbility::StaticClass(), 1, static_cast<int32>(EAbilityInput::PrimaryAction), this);

	if (ensure(AbilitySystemComponent))
	{
		AbilitySystemComponent->GiveAbility(Spec);
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
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
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

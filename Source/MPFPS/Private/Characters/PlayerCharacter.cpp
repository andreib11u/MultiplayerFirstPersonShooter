// Copyright Andrei Bondarenko 2023

#include "Characters/PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "GameplayAbilitySystem/Abilities/FPSGameplayAbility.h"
#include "GameplayFramework/FPSPlayerState.h"
#include "Input/InputDataAsset.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Subsystems/FindActorsOfClassSubsystem.h"
#include "Types/CollisionTypes.h"
#include "Types/FPSGameplayAbilityTypes.h"
#include "UI/FPSHUD.h"
#include "UI/HUDWidget.h"
#include "Weapons/Weapon.h"
#include "Weapons/EquipmentComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerCharacter, All, All);

APlayerCharacter::APlayerCharacter()
{
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("FirstPersonCamera");
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->SetRelativeLocation(FVector{ -10.f, 0.f, 60.f });
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>("FirstPersonMesh");
	FirstPersonMesh->SetRelativeLocation(FVector{ -30.f, 0.f, -151.32f });
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

	ThirdPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>("ThirdPersonCameraBoom");
	ThirdPersonCameraBoom->SetupAttachment(GetCapsuleComponent());
	ThirdPersonCameraBoom->bUsePawnControlRotation = true;

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>("ThirdPersonCamera");
	ThirdPersonCamera->SetupAttachment(ThirdPersonCameraBoom);
	ThirdPersonCamera->SetActive(false);

	ReviveInteraction = CreateDefaultSubobject<UInteractionComponent>("ReviveInteraction");
	ReviveInteraction->SetupAttachment(GetCapsuleComponent());
	ReviveInteraction->SetCollisionResponseToChannel(INTERACTION_TRACE_COLLISION, ECR_Ignore);
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
		EnhancedInputComponent->BindAction(InputActions->SecondaryAction, ETriggerEvent::Started, this, &APlayerCharacter::SecondaryActionPressed);
		EnhancedInputComponent->BindAction(InputActions->SecondaryAction, ETriggerEvent::Completed, this, &APlayerCharacter::SecondaryActionReleased);
	}

	if (InputActions->Reload)
	{
		EnhancedInputComponent->BindAction(InputActions->Reload, ETriggerEvent::Started, this, &APlayerCharacter::ReloadPressed);
		EnhancedInputComponent->BindAction(InputActions->Reload, ETriggerEvent::Completed, this, &APlayerCharacter::ReloadReleased);
	}

	if (InputActions->Use)
	{
		EnhancedInputComponent->BindAction(InputActions->Use, ETriggerEvent::Started, this, &APlayerCharacter::UsePressed);
		EnhancedInputComponent->BindAction(InputActions->Use, ETriggerEvent::Completed, this, &APlayerCharacter::UseReleased);
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

	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &APlayerCharacter::OnGameplayEffectAdded);
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &APlayerCharacter::OnGameplayEffectRemoved);

	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &APlayerCharacter::OnGameplayEffectAdded);
	AbilitySystemComponent
		->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Character.State.Recovering"), EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &APlayerCharacter::OnReviving);

	GrantAbilities();

	InitializeAttributes();
}

// void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(APlayerCharacter, CharacterAmmo);
// }

void APlayerCharacter::OnItemChanged(UEquippableItem* Item)
{
	if (Item)
	{
		FirstPersonWeaponMeshComponent->SetSkeletalMesh(Item->GetItemMesh());
		FirstPersonWeaponMeshComponent->SetRelativeLocation(Item->FirstPersonLocation);
		FirstPersonWeaponMeshComponent->SetRelativeRotation(Item->FirstPersonRotation);

		ThirdPersonWeaponMeshComponent->SetSkeletalMesh(Item->GetItemMesh());
		ThirdPersonWeaponMeshComponent->SetRelativeLocation(Item->ThirdPersonLocation);
		ThirdPersonWeaponMeshComponent->SetRelativeRotation(Item->ThirdPersonRotation);

		if (IsLocallyControlled())
		{
			FirstPersonWeaponMeshComponent->SetVisibility(true, true);

			FirstPersonMesh->SetRelativeLocation(Item->ArmsMeshRelativeLocation);
		}
		else
		{
			ThirdPersonWeaponMeshComponent->SetVisibility(true, true);
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
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &APlayerCharacter::OnGameplayEffectAdded);
	AbilitySystemComponent
		->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Character.State.Recovering"), EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &APlayerCharacter::OnReviving);
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &APlayerCharacter::OnGameplayEffectRemoved);

	InitializeAttributes();
}

void APlayerCharacter::SetFirstPersonMeshVisibility()
{
	GetMesh()->SetVisibility(false, true);
	FirstPersonMesh->SetVisibility(true, true);
}

void APlayerCharacter::SetThirdPersonMeshVisibility()
{
	GetMesh()->SetVisibility(true, true);
	FirstPersonMesh->SetVisibility(false, true);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		SetFirstPersonMeshVisibility();
	}
	else
	{
		SetThirdPersonMeshVisibility();
	}

	auto FindActorsSubsystem = GetWorld()->GetSubsystem<UFindActorsOfClassSubsystem>();
	if (FindActorsSubsystem)
	{
		FindActorsSubsystem->AddActor(APlayerCharacter::StaticClass(), this);
	}
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	auto FindActorsSubsystem = GetWorld()->GetSubsystem<UFindActorsOfClassSubsystem>();
	if (FindActorsSubsystem)
	{
		FindActorsSubsystem->RemoveActor(APlayerCharacter::StaticClass(), this);
	}
}

#if WITH_EDITOR
void APlayerCharacter::ShowDebugSpreadCone(float HalfConeDeg)
{
	for (int32 ThetaDeg = 0; ThetaDeg < 360; ++ThetaDeg)
	{
		FVector StartTrace = GetFirstPersonCamera()->GetComponentLocation();
		StartTrace.X -= 1.f;
		// const FVector DeviationVector = FMath::VRandCone(GetBaseAimRotation().Vector(), FMath::DegreesToRadians(45.f));

		// get axes we need to rotate around
		FMatrix const DirMat = FRotationMatrix(GetBaseAimRotation());
		// note the axis translation, since we want the variation to be around X
		FVector const DirZ = DirMat.GetScaledAxis(EAxis::X);
		FVector const DirY = DirMat.GetScaledAxis(EAxis::Y);

		FVector Result = GetBaseAimRotation().Vector().RotateAngleAxis(HalfConeDeg, DirY);
		Result = Result.RotateAngleAxis(ThetaDeg, DirZ);

		// ensure it's a unit vector (might not have been passed in that way)
		Result = Result.GetSafeNormal();

		const FVector EndTrace = StartTrace + Result * 9999.f;

		TArray<AActor*> ActorsToIgnore;
		/*ActorsToIgnore.Add(Character);*/
		TArray<FHitResult> Results;

		const ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(BULLET_TRACE_COLLISION);

		TArray<FHitResult> LineTraceMultiResults;
		UKismetSystemLibrary::LineTraceMulti(GetWorld(), StartTrace, EndTrace, TraceType, true, ActorsToIgnore, EDrawDebugTrace::ForOneFrame,
											 LineTraceMultiResults, true, FLinearColor::Yellow);
	}
}
#endif // WITH_EDITOR

void APlayerCharacter::OnGameplayEffectAdded(UAbilitySystemComponent* InAbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec,
											 FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	if (AimingEffect)
	{
		if (GameplayEffectSpec.Def->IsA(AimingEffect))
		{
			auto CurrentWeapon = Cast<UWeapon>(EquipmentComponent->GetCurrentItem());
			if (CurrentWeapon)
			{
				int32 EffectStackCount = AbilitySystemComponent->GetGameplayEffectCount(GameplayEffectSpec.Def->GetClass(), nullptr);
				if (EffectStackCount > 0)
				{
					FirstPersonMesh->SetRelativeLocation(CurrentWeapon->ArmsMeshRelativeLocationWhenAiming);
				}
			}
		}
	}
}

void APlayerCharacter::OnGameplayEffectRemoved(const FActiveGameplayEffect& ActiveGameplayEffect)
{
	if (AimingEffect && ActiveGameplayEffect.Spec.Def->IsA(AimingEffect))
	{
		auto CurrentWeapon = Cast<UWeapon>(EquipmentComponent->GetCurrentItem());
		if (CurrentWeapon)
		{
			int32 EffectStackCount = AbilitySystemComponent->GetGameplayEffectCount(ActiveGameplayEffect.Spec.Def->GetClass(), nullptr);
			if (EffectStackCount < 1)
			{
				FirstPersonMesh->SetRelativeLocation(CurrentWeapon->ArmsMeshRelativeLocation);
			}
		}
	}
}

void APlayerCharacter::RecoveredFromDying()
{
	GetMesh()->SetCollisionResponseToChannel(BULLET_TRACE_COLLISION, ECR_Overlap);
	AbilitySystemComponent->SetLooseGameplayTagCount(FGameplayTag::RequestGameplayTag("Character.State.Downed"), 0);

	if (IsLocallyControlled())
	{
		SetFirstPersonMeshVisibility();

		FirstPersonCamera->SetActive(true);
		ThirdPersonCamera->SetActive(false);
	}

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	bUseControllerRotationYaw = true;

	ReviveInteraction->SetCollisionResponseToChannel(INTERACTION_TRACE_COLLISION, ECR_Ignore);
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bMoveTPCamera)
	{
		const float TargetArmLength = bMoveCameraForward ? InitialArmLengthTPCamera : TargetArmLengthTPCamera;

		ThirdPersonCameraBoom->TargetArmLength =
			FMath::FInterpConstantTo(ThirdPersonCameraBoom->TargetArmLength, TargetArmLength, DeltaSeconds, MoveCameraSpeed);

		if (FMath::IsNearlyEqual(ThirdPersonCameraBoom->TargetArmLength, TargetArmLength))
		{
			bMoveTPCamera = false;
			OnCameraFullyMoved.Broadcast();
		}
	}
}

void APlayerCharacter::InitializeAttributes()
{
	Super::InitializeAttributes();
}

void APlayerCharacter::GrantAbilities()
{
	Super::GrantAbilities();

	if (AbilitySystemComponent && InitialWeaponAbility)
	{
		auto AbilityCDO = Cast<UFPSGameplayAbility>(InitialWeaponAbility->GetDefaultObject());
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(InitialWeaponAbility, 1, static_cast<int32>(AbilityCDO->GetAbilityInput()), this);

		AbilitySystemComponent->GiveAbility(Spec);
	}
}

void APlayerCharacter::StartMovingTPCamera(bool bMoveForward)
{
	if (!bMoveForward)
	{
		FirstPersonCamera->SetActive(false);
		ThirdPersonCamera->SetActive(true);
	}

	ThirdPersonCameraBoom->TargetArmLength = bMoveForward ? TargetArmLengthTPCamera : InitialArmLengthTPCamera;
	bMoveTPCamera = true;
	bMoveCameraForward = bMoveForward;
}

void APlayerCharacter::OnZeroHealth()
{
	GetMesh()->SetCollisionResponseToChannel(BULLET_TRACE_COLLISION, ECR_Ignore);
	AbilitySystemComponent->SetLooseGameplayTagCount(FGameplayTag::RequestGameplayTag("Character.State.Downed"), 1);

	StartMovingTPCamera(false);
	if (IsLocallyControlled())
	{
		SetThirdPersonMeshVisibility();
	}

	GetCharacterMovement()->DisableMovement();
	bUseControllerRotationYaw = false;

	ReviveInteraction->SetCollisionResponseToChannel(INTERACTION_TRACE_COLLISION, ECR_Block);
}

void APlayerCharacter::OnReviving(FGameplayTag GameplayTag, int32 Count)
{
	if (Count > 0)
	{
		StartMovingTPCamera(true);
	}
	else
	{
		RecoveredFromDying();
	}
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
// todo: change on two functions with payload instead
void APlayerCharacter::PrimaryActionPressed()
{
	AbilitySystemComponent->AbilityLocalInputPressedQueued(static_cast<int32>(EAbilityInput::PrimaryAction));
}

void APlayerCharacter::PrimaryActionReleased()
{
	AbilitySystemComponent->AbilityLocalInputReleasedQueued(static_cast<int32>(EAbilityInput::PrimaryAction));
}

void APlayerCharacter::SecondaryActionPressed()
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EAbilityInput::SecondaryAction));
}

void APlayerCharacter::SecondaryActionReleased()
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

void APlayerCharacter::UsePressed()
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EAbilityInput::Interact));
}

void APlayerCharacter::UseReleased()
{
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(EAbilityInput::Interact));
}

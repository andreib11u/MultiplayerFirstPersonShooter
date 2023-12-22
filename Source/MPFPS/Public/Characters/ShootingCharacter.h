// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "AbilitySystemInterface.h"
#include "Weapons/Weapon.h"
#include "ShootingCharacter.generated.h"

enum class EAbilityInput;
class UInteractionComponent;
class USpringArmComponent;
class UPlayerAttributeSet;
class UFPSGameplayAbility;
class UEquipmentComponent;
class UGameplayAbility;
class UGameplayEffect;
class UFPSAbilitySystemComponent;
struct FInputActionValue;
class UAbilitySystemComponent;
class UCameraComponent;
class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraFullyMoved);

/**
 *
 */
UCLASS()
class MPFPS_API AShootingCharacter : public ABaseCharacter, public IAbilitySystemInterface
{
private:
	GENERATED_BODY()

public:
	AShootingCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCamera; }
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }
	UEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }

	USkeletalMeshComponent* GetFirstPersonWeaponMesh() const { return FirstPersonWeaponMeshComponent; }
	USkeletalMeshComponent* GetThirdPersonWeaponMesh() const { return ThirdPersonWeaponMeshComponent; }
	USkeletalMeshComponent* GetFirstPersonMesh() { return FirstPersonMesh; }

	float GetInteractionLength() const { return InteractionLength; }

	UPROPERTY(EditAnywhere)
	FOnCameraFullyMoved OnCameraFullyMoved;

	void RecoveredFromDying();
	void SetFirstPersonMeshVisibility();
	void SetThirdPersonMeshVisibility();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void InitializeAttributes() override;

	virtual void GrantAbilities() override;
	virtual void OnZeroHealth() override;

	virtual void OnReviving(FGameplayTag GameplayTag, int32 Count);

	void MoveCameraWhenDowned(float DeltaSeconds);

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputDataAsset* InputActions;
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UFPSAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UEquipmentComponent* EquipmentComponent;

	UPROPERTY()
	UPlayerAttributeSet* PlayerAttributeSet;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* FirstPersonMesh;
	UPROPERTY(EditAnywhere)
	UCameraComponent* FirstPersonCamera;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* FirstPersonWeaponMeshComponent;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* ThirdPersonWeaponMeshComponent;
	UPROPERTY(EditAnywhere)
	USpringArmComponent* ThirdPersonCameraBoom;
	UPROPERTY(EditAnywhere)
	UCameraComponent* ThirdPersonCamera;
	UPROPERTY(EditAnywhere)
	UInteractionComponent* ReviveInteraction;

	UPROPERTY(EditAnywhere)
	float InitialArmLengthTPCamera = 50.f;
	UPROPERTY(EditAnywhere)
	float TargetArmLengthTPCamera = 300.f;
	UPROPERTY(EditAnywhere)
	float MoveCameraSpeed = 200.f;

	bool bMoveTPCamera = false;
	bool bMoveCameraForward = false;

	UPROPERTY(EditAnywhere)
	float InteractionLength = 120.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> AimingEffect;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UFPSGameplayAbility> InitialWeaponAbility;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> ReviveEffect;

	UFUNCTION()
	void OnItemChanged(UEquippableItem* Item);

#if WITH_EDITOR
	void ShowDebugSpreadCone(float HalfConeDeg);
#endif

	void OnGameplayEffectAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec,
							   FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
	void OnGameplayEffectRemoved(const FActiveGameplayEffect& ActiveGameplayEffect);

	void StartMovingTPCamera(bool bMoveForward);

	// Input callbacks

	void Look(const FInputActionValue& InputActionValue);
	void Move(const FInputActionValue& InputActionValue);

	void QueuedAbilityActionPressed(EAbilityInput AbilityInput);
	void QueuedAbilityActionReleased(EAbilityInput AbilityInput);
	void AbilityActionPressed(EAbilityInput AbilityInput);
	void AbilityActionReleased(EAbilityInput AbilityInput);
};

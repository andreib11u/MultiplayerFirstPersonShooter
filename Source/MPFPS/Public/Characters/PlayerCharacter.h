// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "AbilitySystemInterface.h"
#include "Weapons/Weapon.h"
#include "PlayerCharacter.generated.h"

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

/**
 *
 */
UCLASS()
class MPFPS_API APlayerCharacter : public ABaseCharacter, public IAbilitySystemInterface
{
private:
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCamera; }
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }
	UEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }

	USkeletalMeshComponent* GetFirstPersonWeaponMesh() const { return FirstPersonWeaponMeshComponent; }
	USkeletalMeshComponent* GetThirdPersonWeaponMesh() const { return ThirdPersonWeaponMeshComponent; }

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void InitializeAttributes() override;

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
	TSubclassOf<UGameplayEffect> AimingEffect;

	UFUNCTION()
	void OnItemChanged(UEquippableItem* Item);

#if WITH_EDITOR
	void ShowDebugSpreadCone(float HalfConeDeg);
#endif

	void OnGameplayEffectAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec,
							   FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
	void OnGameplayEffectRemoved(const FActiveGameplayEffect& ActiveGameplayEffect);

	// Input callbacks

	void Look(const FInputActionValue& InputActionValue);
	void Move(const FInputActionValue& InputActionValue);

	void PrimaryActionPressed();
	void PrimaryActionReleased();
	void SecondaryActionPressed();
	void SecondaryActionReleased();
	void ReloadPressed();
	void ReloadReleased();
};

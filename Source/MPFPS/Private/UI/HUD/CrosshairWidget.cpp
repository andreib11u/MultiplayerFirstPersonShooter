// Copyright Andrei Bondarenko 2023

#include "UI/HUD/CrosshairWidget.h"

#include "Camera/CameraComponent.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MPFPS/MPFPS.h"
#include "Types/CollisionTypes.h"
#include "Weapons/EquipmentComponent.h"

void UCrosshairWidget::Init(UEquipmentComponent* Equipment)
{
	if (!Equipment)
	{
		UE_LOG(LogUI, Error, TEXT("CrosshairWidget can't be initialized: EquipmentComponent is null"));
		return;
	}

	EquipmentComponent = Equipment;
	EquipmentComponent->OnSpreadAdded.BindUObject(this, &UCrosshairWidget::OnSpreadAdded);

	PlayerCharacter = GetOwningPlayer()->GetPawn<APlayerCharacter>();
	check(PlayerCharacter);

	SetTargetSpread(EquipmentComponent->GetSpread());
}

void UCrosshairWidget::SetTargetSpread(float InSpread)
{
	TargetSpread = InSpread;
}

void UCrosshairWidget::AddSpread(float InSpread)
{
	CurrentSpread += InSpread;
}

void UCrosshairWidget::ChangeColor(FLinearColor Color)
{
	if (CrosshairColor != Color)
	{
		CrosshairColor = Color;
		RightImage->SetColorAndOpacity(Color);
		LeftImage->SetColorAndOpacity(Color);
		UpImage->SetColorAndOpacity(Color);
		DownImage->SetColorAndOpacity(Color);
	}
}

void UCrosshairWidget::UpdateColorWithTargetAttitude()
{
	FVector PlayerCameraLocation = PlayerCharacter->GetFirstPersonCamera()->GetComponentLocation();
	float Range = EquipmentComponent->GetWeaponStats().Range;
	FVector PlayerCameraForwardVector = PlayerCharacter->GetBaseAimRotation().Vector();

	FVector TraceStart = PlayerCameraLocation;
	FVector TraceEnd = PlayerCameraLocation + PlayerCameraForwardVector * Range;
	const ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(BULLET_TRACE_COLLISION);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PlayerCharacter);
	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::LineTraceMulti(GetWorld(), TraceStart, TraceEnd, TraceType, true, ActorsToIgnore, EDrawDebugTrace::None,
	                                                  HitResults, true);

	ChangeColor(FLinearColor::White);

	if (!HitResults.IsEmpty())
	{
		if (auto GenericTeamAgentInterface = Cast<IGenericTeamAgentInterface>(PlayerCharacter))
		{
			if (GenericTeamAgentInterface->GetTeamAttitudeTowards(*HitResults[0].GetActor()) == ETeamAttitude::Hostile)
			{
				ChangeColor(FLinearColor::Red);
			}
			if (GenericTeamAgentInterface->GetTeamAttitudeTowards(*HitResults[0].GetActor()) == ETeamAttitude::Friendly)
			{
				ChangeColor(FLinearColor::Green);
			}
		}
	}
}

void UCrosshairWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TargetSpread = EquipmentComponent->GetSpread();

	CurrentSpread = FMath::FInterpConstantTo(CurrentSpread, TargetSpread, InDeltaTime, SpreadAdjustmentSpeed);

	CalculateCrosshairPosition();

	UpdateColorWithTargetAttitude();
}

void UCrosshairWidget::CalculateCrosshairPosition()
{
	// todo cycle stat

	const FGeometry& Geometry = GetCachedGeometry();
	auto LocalSize = Geometry.GetLocalSize();

	auto CanvasPanelSlot = Cast<UCanvasPanelSlot>(Right->Slot);
	FVector2D CrosshairHorizontalElementSize = CanvasPanelSlot->GetSize();
	CanvasPanelSlot = Cast<UCanvasPanelSlot>(Up->Slot);
	FVector2D CrosshairVerticalElementSize = CanvasPanelSlot->GetSize();

	// calculate min and max crosshair positions
	RightPositions = TInterval<float>(0.f, LocalSize.X * 0.5 - CrosshairHorizontalElementSize.X);
	LeftPositions = TInterval<float>(-CrosshairHorizontalElementSize.X, -LocalSize.X * 0.5 - CrosshairHorizontalElementSize.X);
	UpPositions = TInterval<float>(-CrosshairVerticalElementSize.Y, -LocalSize.Y * 0.5 - CrosshairVerticalElementSize.Y);
	DownPositions = TInterval<float>(0.f, LocalSize.Y * 0.5 - CrosshairVerticalElementSize.Y);

	// set crosshair element positions
	Up->SetRenderTranslation(FVector2D{ 0.f, FMath::Lerp(UpPositions.Min, UpPositions.Max, CurrentSpread) });
	Down->SetRenderTranslation(FVector2D{ 0.f, FMath::Lerp(DownPositions.Min, DownPositions.Max, CurrentSpread) });
	Right->SetRenderTranslation(FVector2D{ FMath::Lerp(RightPositions.Min, RightPositions.Max, CurrentSpread), 0.f });
	Left->SetRenderTranslation(FVector2D{ FMath::Lerp(LeftPositions.Min, LeftPositions.Max, CurrentSpread), 0.f });
}

void UCrosshairWidget::OnSpreadAdded(float AddedSpread)
{
	AddSpread(AddedSpread);
}

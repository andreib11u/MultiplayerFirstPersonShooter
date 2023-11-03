// Copyright Andrei Bondarenko 2023

#include "UI/HUD/CrosshairWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "MPFPS/MPFPS.h"
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

void UCrosshairWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TargetSpread = EquipmentComponent->GetSpread();

	CurrentSpread = FMath::FInterpConstantTo(CurrentSpread, TargetSpread, InDeltaTime, SpreadAdjustmentSpeed);

	CalculateCrosshairPosition();
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
	RightPositions = TInterval<float>( 0.f, LocalSize.X * 0.5 - CrosshairHorizontalElementSize.X );
	LeftPositions = TInterval<float>( -CrosshairHorizontalElementSize.X, -LocalSize.X * 0.5 - CrosshairHorizontalElementSize.X );
	UpPositions = TInterval<float>( -CrosshairVerticalElementSize.Y, -LocalSize.Y * 0.5 - CrosshairVerticalElementSize.Y );
	DownPositions = TInterval<float>( 0.f, LocalSize.Y * 0.5 - CrosshairVerticalElementSize.Y );

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

// Copyright Andrei Bondarenko 2023

#include "UI/HUD/DamageDirectionIndicatorWidget.h"

void UDamageDirectionIndicatorWidget::CalculateTransform()
{
	if (DamageCauser.IsValid() && OriginActor.IsValid())
	{
		const FVector LookDirection = OriginActor->GetActorForwardVector();
		FVector DamageDirection = DamageCauser->GetActorLocation() - OriginActor->GetActorLocation();

		const float AbsoluteLookAngle = FMath::Atan2(LookDirection.Y, LookDirection.X);
		const float AbsoluteDamageAngle = FMath::Atan2(DamageDirection.Y, DamageDirection.X);

		const float DamageAngle = AbsoluteDamageAngle - AbsoluteLookAngle;

		SetRenderTransformAngle(FMath::RadiansToDegrees(DamageAngle));

		int32 ViewportSizeX;
		int32 ViewportSizeY;
		GetOwningPlayer()->GetViewportSize(ViewportSizeX, ViewportSizeY);

		const float X = FMath::Sin(DamageAngle) * ViewportSizeX * DistanceFromCenter;
		const float Y = FMath::Cos(DamageAngle) * ViewportSizeY * DistanceFromCenter;
		FVector2D WidgetDirection = FVector2D{ X, -Y };

		SetRenderTranslation(WidgetDirection);
	}
	else
	{
		bActive = false;
		RemoveFromParent();
	}
}

void UDamageDirectionIndicatorWidget::Init(AActor* InOriginActor, AActor* InDamageCauser)
{
	bActive = true;
	DamageCauser = InDamageCauser;
	OriginActor = InOriginActor;
	CalculateTransform();

	GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle, this, &UDamageDirectionIndicatorWidget::RemoveAndDeactivate, DisappearTime);
}

void UDamageDirectionIndicatorWidget::RemoveAndDeactivate()
{
	bActive = false;
	RemoveFromParent();
}

void UDamageDirectionIndicatorWidget::ResetTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(DisappearTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(DisappearTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle, this, &UDamageDirectionIndicatorWidget::RemoveAndDeactivate, DisappearTime);
	}
}

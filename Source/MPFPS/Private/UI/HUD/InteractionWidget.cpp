// Copyright Andrei Bondarenko 2023

#include "UI/HUD/InteractionWidget.h"
#include "Camera/CameraComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Components/InteractionComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/CollisionTypes.h"

void UInteractionWidget::Init(APlayerCharacter* InPlayerCharacter)
{
	PlayerCharacter = InPlayerCharacter;
}

void UInteractionWidget::ShowInteraction(const FText& InInteractionText)
{
	SetRenderOpacity(1.f);
	InteractionText->SetText(InInteractionText);
}

void UInteractionWidget::HideInteraction()
{
	SetRenderOpacity(0.f);
	InteractionProgress->SetPercent(0.f);
}

void UInteractionWidget::SetInteractionProgress(float Percent)
{
	InteractionProgress->SetPercent(Percent);
}

void UInteractionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FVector PlayerCameraLocation = PlayerCharacter->GetFirstPersonCamera()->GetComponentLocation();
	const FVector PlayerCameraForwardVector = PlayerCharacter->GetBaseAimRotation().Vector();

	FVector TraceStart = PlayerCameraLocation;
	FVector TraceEnd = PlayerCameraLocation + PlayerCameraForwardVector * InteractionRange;
	const ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(INTERACTION_TRACE_COLLISION);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PlayerCharacter.Get());
	FHitResult HitResult;

	bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStart, TraceEnd, TraceType, true, ActorsToIgnore, EDrawDebugTrace::None,
													  HitResult, true);

	if (bHit)
	{
		auto LookAtInteractionComponent = Cast<UInteractionComponent>(HitResult.GetComponent());

		if (!LookAtInteractionComponent)
		{
			HideInteraction();
			return;
		}

		if (LookAtInteractionComponent == InteractionComponent)
		{
			ShowInteraction(LookAtInteractionComponent->GetInteractionText());
			SetInteractionProgress(InteractionComponent->GetProgress());
			return;
		}

		InteractionComponent = LookAtInteractionComponent;
		ShowInteraction(InteractionComponent->GetInteractionText());
		SetInteractionProgress(InteractionComponent->GetProgress());
		return;
	}

	HideInteraction();
}

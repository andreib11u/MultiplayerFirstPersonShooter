// Copyright Andrei Bondarenko 2023


#include "UI/HUD/MoneyAdditionWidget.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/TextBlock.h"

void UMoneyAdditionWidget::Init(int32 Money)
{
	MoneyText->SetText(FText::AsNumber(Money));
	CurrentMoney = Money;

	GetWorld()->GetTimerManager().SetTimer(FloatingTimerHandle, this, &UMoneyAdditionWidget::OnFloatingTimerExpired, TimeFloating);
}

void UMoneyAdditionWidget::AddMoney(int32 Money)
{
	MoneyText->SetText(FText::AsNumber(CurrentMoney + Money));

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	TimerManager.ClearTimer(FloatingTimerHandle);
	TimerManager.SetTimer(FloatingTimerHandle, this, &UMoneyAdditionWidget::OnFloatingTimerExpired, TimeFloating);
}

void UMoneyAdditionWidget::OnAdditionAnimationFinished(UUMGSequencePlayer& USequencePlayer)
{
	OnMoneyAnimationEnded.Broadcast(CurrentMoney);
	RemoveFromParent();
}

void UMoneyAdditionWidget::OnFloatingTimerExpired()
{
	bAnimationStarted = true;

	SequencePlayer = PlayAnimation(StartAnimation);

	SequencePlayer->OnSequenceFinishedPlaying().AddUObject(this, &UMoneyAdditionWidget::OnAdditionAnimationFinished);
}

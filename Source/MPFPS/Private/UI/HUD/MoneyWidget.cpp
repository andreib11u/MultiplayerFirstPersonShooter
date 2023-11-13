// Copyright Andrei Bondarenko 2023


#include "UI/HUD/MoneyWidget.h"

#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "GameplayAbilitySystem/AttributeSets/PlayerAttributeSet.h"
#include "MPFPS/MPFPS.h"
#include "UI/HUD/MoneyAdditionWidget.h"

void UMoneyWidget::OnMoneyChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	const float ValueAdded = OnAttributeChangeData.NewValue - OnAttributeChangeData.OldValue;

	if (!MoneyAdditionWidget || (MoneyAdditionWidget && MoneyAdditionWidget->IsAnimationStarted()))
	{
		MoneyAdditionWidget = CreateWidget<UMoneyAdditionWidget>(this, MoneyAdditionWidgetClass);
		MoneyAdditionWidget->Init(FMath::RoundToInt32(ValueAdded));
		MoneyAdditionWidget->OnMoneyAnimationEnded.AddDynamic(this, &UMoneyWidget::MoneyAdditionAnimationEnded);
		OverlayPanel->AddChildToOverlay(MoneyAdditionWidget);

		FVector2D RenderTranslation = MoneyAdditionWidget->GetRenderTransform().Translation;
		RenderTranslation.Y += InitialRenderTranslationY;
		MoneyAdditionWidget->SetRenderTranslation(RenderTranslation);
		return;
	}

	MoneyAdditionWidget->AddMoney(FMath::RoundToInt32(ValueAdded));
}

void UMoneyWidget::SetMoney(float Money)
{
	CurrentMoney = FMath::RoundToInt32(Money);
	MoneyNumberText->SetText(FText::AsNumber(CurrentMoney));
}

void UMoneyWidget::Init(UFPSAbilitySystemComponent* AbilitySystemComponent)
{
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogUI, Error, TEXT("MoneyWidget can't be initialized: AbilitySystemComponent is null"));
		return;
	}

	auto AttributeSet = AbilitySystemComponent->GetAttributeSet<UPlayerAttributeSet>();

	if (!AttributeSet)
	{
		UE_LOG(LogUI, Error, TEXT("MoneyWidget can't be initialized: there is no AttributeSet with a type %s in passed AbilitySystemComponent"),
			   *UBaseAttributeSet::StaticClass()->GetName());
		return;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMoneyAttribute())
		.AddUObject(this, &UMoneyWidget::OnMoneyChanged);

	SetMoney(AttributeSet->GetMoney());

}

void UMoneyWidget::MoneyAdditionAnimationEnded(int32 MoneyToAdd)
{
	SetMoney(CurrentMoney + MoneyToAdd);
	MoneyAdditionWidget = nullptr;
}

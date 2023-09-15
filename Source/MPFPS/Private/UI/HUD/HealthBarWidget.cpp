// Copyright Andrei Bondarenko 2023

#include "UI/HUD/HealthBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameplayAbilitySystem/FPSAbilitySystemComponent.h"
#include "GameplayAbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "MPFPS/MPFPS.h"

void UHealthBarWidget::Init(UFPSAbilitySystemComponent* AbilitySystemComponent)
{
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogUI, Error, TEXT("HealthBar can't be initialized: AbilitySystemComponent is null"));
		return;
	}

	auto AttributeSet = AbilitySystemComponent->GetAttributeSet<UBaseAttributeSet>();

	if (!AttributeSet)
	{
		UE_LOG(LogUI, Error, TEXT("HealthBar can't be initialized: there is no AttributeSet with a type %s in passed AbilitySystemComponent"),
			   *UBaseAttributeSet::StaticClass()->GetName());
		return;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentHealthAttribute())
		.AddUObject(this, &UHealthBarWidget::OnCurrentHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute())
		.AddUObject(this, &UHealthBarWidget::OnMaxHealthChanged);

	SetMaxHealth(AttributeSet->GetMaxHealth());
	SetCurrentHealth(AttributeSet->GetCurrentHealth());
}

void UHealthBarWidget::SetCurrentHealth(const float CurrentHealth)
{
	CachedCurrentHealth = CurrentHealth;
	UpdateProgressBarPercent();
	UpdateHealthBarText();
}

void UHealthBarWidget::SetMaxHealth(const float MaxHealth)
{
	CachedMaxHealth = MaxHealth;
	UpdateProgressBarPercent();
	UpdateHealthBarText();
}

void UHealthBarWidget::OnCurrentHealthChanged(const FOnAttributeChangeData& OnCurrentHealthChangeData)
{
	SetCurrentHealth(OnCurrentHealthChangeData.NewValue);
}

void UHealthBarWidget::OnMaxHealthChanged(const FOnAttributeChangeData& OnMaxHealthChangeData)
{
	SetMaxHealth(OnMaxHealthChangeData.NewValue);
}

void UHealthBarWidget::UpdateProgressBarPercent()
{
	check(CachedMaxHealth != 0.f);

	const float Percent = CachedCurrentHealth / CachedMaxHealth;
	HealthProgressBar->SetPercent(Percent);
	HealthProgressBar->SetFillColorAndOpacity(FColor::MakeRedToGreenColorFromScalar(Percent).ReinterpretAsLinear());
}

void UHealthBarWidget::UpdateHealthBarText()
{
	const FText HealthBarText = FText::Format(NSLOCTEXT("UI", "HealthText", "{0}/{1}"), //
											  FText::AsNumber(CachedCurrentHealth), //
											  FText::AsNumber(CachedMaxHealth));

	HealthText->SetText(HealthBarText);
}


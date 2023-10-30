// Copyright Andrei Bondarenko 2023


#include "Characters/Enemies/Zombie.h"
#include "Components/AudioComponent.h"
#include "GameplayAbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "Kismet/GameplayStatics.h"

AZombie::AZombie()
{
	DefaultAttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("BaseAttributeSet");
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);

	RandomStream.GenerateNewSeed();
}

void AZombie::BeginPlay()
{
	Super::BeginPlay();

	PlaySound();
}

void AZombie::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(AudioTimerHandle);
}

void AZombie::PlaySound()
{
	AudioComponent->Play();

	float Deviation = RandomStream.FRandRange(-IntervalRandomDeviation, IntervalRandomDeviation);
	GetWorld()->GetTimerManager().SetTimer(AudioTimerHandle, this, &AZombie::PlaySound, AudioInterval + Deviation);
}

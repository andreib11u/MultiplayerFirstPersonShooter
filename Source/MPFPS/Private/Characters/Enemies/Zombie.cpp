// Copyright Andrei Bondarenko 2023


#include "Characters/Enemies/Zombie.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/ShootingCharacter.h"
#include "Components/AudioComponent.h"
#include "GameplayAbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/FindActorsOfClassSubsystem.h"

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

	auto FindActorsSubsystem = GetWorld()->GetSubsystem<UFindActorsOfClassSubsystem>();
	if (FindActorsSubsystem)
	{
		FindActorsSubsystem->AddActor(AZombie::StaticClass(), this);
	}

	auto AIController = GetController<AAIController>();
	if (AIController)
	{
		TArray<AActor*> Players = FindActorsSubsystem->FindAllActors(AShootingCharacter::StaticClass());
		if (!Players.IsEmpty())
		{
			AIController->GetBlackboardComponent()->SetValueAsObject("TargetEnemy", Players[0]);
		}
		else
		{
			PlayerSpawnedDelegateHandle = FindActorsSubsystem->OnNewFindableActorAdded.AddUObject(this, &AZombie::OnPlayerSpawned);
		}
	}

	PlaySound();
}

void AZombie::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(AudioTimerHandle);

	auto FindActorsSubsystem = GetWorld()->GetSubsystem<UFindActorsOfClassSubsystem>();
	if (FindActorsSubsystem)
	{
		FindActorsSubsystem->RemoveActor(AZombie::StaticClass(), this);
	}
}

void AZombie::PlaySound()
{
	AudioComponent->Play();

	float Deviation = RandomStream.FRandRange(-IntervalRandomDeviation, IntervalRandomDeviation);
	GetWorld()->GetTimerManager().SetTimer(AudioTimerHandle, this, &AZombie::PlaySound, AudioInterval + Deviation);
}

void AZombie::OnPlayerSpawned(UClass* Class, AActor* Actor)
{
	if (Class == AShootingCharacter::StaticClass())
	{
		auto FindActorsSubsystem = GetWorld()->GetSubsystem<UFindActorsOfClassSubsystem>();
		if (!FindActorsSubsystem)
		{
			return;
		}

		auto AIController = GetController<AAIController>();
		if (AIController)
		{
			AIController->GetBlackboardComponent()->SetValueAsObject("TargetEnemy", Actor);
			FindActorsSubsystem->OnNewFindableActorAdded.Remove(PlayerSpawnedDelegateHandle);
		}
	}
}

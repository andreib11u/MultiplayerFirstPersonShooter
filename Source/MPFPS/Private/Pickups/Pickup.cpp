// Copyright Andrei Bondarenko 2023

#include "Pickups/Pickup.h"
#include "Characters/PlayerCharacter.h"
#include "Components/SphereComponent.h"

void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                             bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto OverlappedPlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		OnPickup(OverlappedPlayerCharacter);
		Destroy();
	}
}

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	Collision->SetupAttachment(Mesh);
	Collision->SetSphereRadius(20.f);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitialLocationZ = GetActorLocation().Z;
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float Time = GetGameTimeSinceCreation();
	const float Sin = FMath::Sin(Time);
	const FVector Location = GetActorLocation();
	SetActorLocation({Location.X, Location.Y, InitialLocationZ + Sin * ZMaxOffset});

	const FRotator Rotation = GetActorRotation();
	SetActorRotation(FRotator{Rotation.Pitch, Rotation.Yaw + DeltaTime * RotationSpeed, Rotation.Roll});
}

void APickup::OnPickup(APlayerCharacter* PlayerCharacter)
{
	
}

// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UInteractionComponent;

UCLASS()
class MPFPS_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:
	ADoor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Open();

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta  = (AllowPrivateAccess = "true"))
	FVector TargetLocation;


	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UInteractionComponent* InteractionComponent;
};

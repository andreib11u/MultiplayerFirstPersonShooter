// Copyright Andrei Bondarenko 2023

#pragma once

#include "CoreMinimal.h"
#include "GameplayFramework/FPSGameInstance.h"
#include "Subsystems/SessionsSubsystem.h"
#include "UI/FPSUserWidget.h"
#include "Types/FPSTypes.h"
#include "CreateSessionWidget.generated.h"

class UOptionChooserWidget;
class UImage;
class USessionWidget;
class UButton;
class UMainMenuWidget;

/**
 * 
 */
UCLASS()
class MPFPS_API UCreateSessionWidget : public UFPSUserWidget
{
	GENERATED_BODY()
public:
	void SetParentMenu(UMainMenuWidget* InParentMenu) { ParentMenu = InParentMenu; }

protected:
	virtual void NativeConstruct() override;

	USessionsSubsystem* GetSessionsSubsystem() const;
private:
	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
	USessionWidget* Session;
	UPROPERTY(meta = (BindWidget))
	UImage* MapImage;
	UPROPERTY(meta = (BindWidget))
	UOptionChooserWidget* MapChooser;

	UPROPERTY()
	UMainMenuWidget* ParentMenu;

	TArray<FMapOptions> Maps;

	TWeakObjectPtr<UFPSGameInstance> GameInstance;

	UFUNCTION()
	void OnSessionCreated(bool bSuccessful);
	void OnSessionStarted(bool bSuccessful);
	UFUNCTION()
	void OnStartButtonClicked();
	UFUNCTION()
	void MapChanged(int32 MapIndex);
};

// Copyright Andrei Bondarenko 2023

#include "UI/MainMenu/CreateSessionWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "GameplayFramework/FPSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MPFPS/MPFPS.h"
#include "Subsystems/SessionsSubsystem.h"
#include "Types/FPSTypes.h"
#include "UI/OptionChooserWidget.h"
#include "UI/MainMenu/SessionWidget.h"

void UCreateSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton->OnClicked.AddDynamic(this, &ThisClass::OnStartButtonClicked);

	GameInstance = GetGameInstance<UFPSGameInstance>();
	if (!GameInstance.IsValid())
	{
		UE_LOG(LogUI, Error, TEXT("Game Instance is not of UFPSGameInstance class"));
		return;
	}

	Maps = GameInstance->GetAvailableMaps();
	if (Maps.IsEmpty())
	{
		UE_LOG(LogUI, Error, TEXT("List of available maps is empty. Set maps in UFPSGameInstance"));
		return;
	}

	TArray<FText> MapNames;
	for (FMapOptions MapOptions : Maps)
	{
		MapNames.Add(MapOptions.MapName);
	}

	MapChooser->SetOptions(MapNames);
	MapChooser->SetOptionIndex(0);
	MapChooser->OnOptionIndexChanged.AddDynamic(this, &ThisClass::MapChanged);

	MapChanged(0);
}

void UCreateSessionWidget::OnSessionStarted(bool bSuccessful)
{
	if (bSuccessful)
	{
		const int32 CurrentMapIndex = MapChooser->GetOptionIndex();
		const int32 CurrentMapModeIndex = Session->GetMapModeChooser()->GetOptionIndex();
		FString MapURLArguments = "listen";

		TMap<FName, TSoftClassPtr<AGameMode>> MapModes = GameInstance->GetMapModes();

		if (TSoftClassPtr<AGameMode>* FoundGameModeClass = MapModes.Find(Maps[CurrentMapIndex].AvailableModes[CurrentMapModeIndex]))
		{
			TSoftClassPtr<AGameMode> GameModeClass = *FoundGameModeClass;
			MapURLArguments.Append("?Game=" + GameModeClass.ToString());
		}

		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Maps[CurrentMapIndex].Map, true, MapURLArguments);
	}
	else
	{
#if WITH_EDITOR
		if (GEngine)
		{
			const FString OnScreenMessage = FString::Printf(TEXT("Couldn't successfully start a session"));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, OnScreenMessage);
		}
#endif // WITH_EDITOR

		StartButton->SetIsEnabled(true);
	}
}

void UCreateSessionWidget::OnSessionCreated(bool bSuccessful)
{
	if (bSuccessful)
	{
		USessionsSubsystem* SessionsSubsystem = GetSessionsSubsystem();
		if (SessionsSubsystem)
		{
			OnSessionStarted(true);
		}
	}
	else
	{
#if WITH_EDITOR
		if (GEngine)
		{
			const FString OnScreenMessage = FString::Printf(TEXT("Couldn't successfully create a session"));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, OnScreenMessage);
		}
#endif // WITH_EDITOR
		StartButton->SetIsEnabled(true);
	}
}

USessionsSubsystem* UCreateSessionWidget::GetSessionsSubsystem() const
{
	if (!GameInstance.IsValid())
	{
		return nullptr;
	}

	auto SessionsSubsystem = GameInstance->GetSubsystem<USessionsSubsystem>();
	if (!SessionsSubsystem)
	{
		return nullptr;
	}

	return SessionsSubsystem;
}

void UCreateSessionWidget::OnStartButtonClicked()
{
	USessionsSubsystem* SessionsSubsystem = GetSessionsSubsystem();
	if (!SessionsSubsystem)
	{
		return;
	}

	SessionsSubsystem->OnCreateSessionCompleteEvent.AddDynamic(this, &ThisClass::OnSessionCreated);
	SessionsSubsystem->CreateSession(8, true);

	StartButton->SetIsEnabled(false);
}

void UCreateSessionWidget::MapChanged(int32 MapIndex)
{
	const FMapOptions& ChosenMap = Maps[MapIndex];
	Session->Display(ChosenMap);

	if (ChosenMap.Image.IsPending())
	{
		UTexture2D* LoadedTexture = ChosenMap.Image.LoadSynchronous();
		MapImage->SetBrushFromTexture(LoadedTexture);
	}
	else
	{
		MapImage->SetBrushFromTexture(ChosenMap.Image.Get());
	}
}

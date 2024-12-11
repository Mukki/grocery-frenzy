// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include "InputCoreTypes.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/InputDeviceLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"

AMultiplayerGameMode::AMultiplayerGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PlayerStarts = TArray<APlayerStart*>();

	Score = 0;
	Timer = 0;
}

void AMultiplayerGameMode::BeginPlay()
{
	Super::BeginPlay();

	CollectAllPlayerStarts();

	UInputDeviceLibrary::GetAllConnectedInputDevices(ConnectedControllers);

	//DEBUG LOG
	UE_LOG(LogTemp, Warning, TEXT("Connected Controllers: %d"), ConnectedControllers.Num());
	UE_LOG(LogTemp, Warning, TEXT("Collected PlayerStarts: %d"), PlayerStarts.Num());

	SpawnCharactersToPlayerStarts();
	UE_LOG(LogTemp, Warning, TEXT("Spawned Characters: %d"), Characters.Num());

	// laisse le temps au niveau de s'initialiser avant de mettre en pause
	GetWorldTimerManager().SetTimer(InitTimer, [this]()
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}, 0.05f, false);
}

void AMultiplayerGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GameStarted && !GameFinished)
	{
		Timer += DeltaTime;
		if (Timer >= GameDurationInSeconds)
		{
			OnGameEnd();
		}
	}
}

void AMultiplayerGameMode::StartGame()
{
	GameStarted = true;
	UGameplayStatics::SetGamePaused(this, false);

}

void AMultiplayerGameMode::OnGameEnd()
{
	GameFinished = true;
	Timer = 0.0;
	UGameplayStatics::SetGamePaused(this, true);
}

void AMultiplayerGameMode::SpawnCharactersToPlayerStarts()
{
	if (PlayerStarts.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No PlayerStarts found!"));
		return;
	}

	for (int32 i = 0; i < ConnectedControllers.Num(); i++)
	{
		if (i >= PlayerStarts.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("Not enough PlayerStarts for all players!"));
			return;
		}

		APawn* Character = GetWorld()->SpawnActor<APawn>(CharacterClass, PlayerStarts[i]->GetActorLocation(),
		                                                 PlayerStarts[i]->GetActorRotation());

		if (auto Controller = UGameplayStatics::GetPlayerController(GetWorld(), i))
		{
			Controller->bAutoManageActiveCameraTarget = false;
			Controller->Possess(Character);
		}
		else
		{
			APlayerController* PlayerController = UGameplayStatics::CreatePlayer(GetWorld(), i, true);
			PlayerController->bAutoManageActiveCameraTarget = false;
			PlayerController->Possess(Character);
		}

		if (Character != nullptr)
		{
			Character->Rename(*FString::Printf(TEXT("Joueur %d"), i + 1));
			Characters.Add(Character);
		}
	}
}

void AMultiplayerGameMode::CollectAllPlayerStarts()
{
	PlayerStarts.Empty();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		APlayerStart* PlayerStart = *It;
		if (PlayerStart != nullptr)
		{
			PlayerStarts.Add(PlayerStart);
		}
	}
}


void AMultiplayerGameMode::SetInputEnabled(const bool bEnableInput)
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	// Parcourir tous les Player Controllers
	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController != nullptr)
		{
			ACharacter* ControlledCharacter = Cast<ACharacter>(PlayerController->GetPawn());

			if (bEnableInput)
			{
				EnableInput(PlayerController);
				if (ControlledCharacter != nullptr)	ControlledCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				UE_LOG(LogTemp, Warning, TEXT("INPUT ENABLED"));
			}
			else
			{
				DisableInput(PlayerController);
				if (ControlledCharacter != nullptr)	ControlledCharacter->GetCharacterMovement()->DisableMovement();
				UE_LOG(LogTemp, Warning, TEXT("INPUT DISABLED"));
			}
		}
	}
}

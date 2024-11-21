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

AMultiplayerGameMode::AMultiplayerGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
    PlayerStarts = TArray<APlayerStart*>();
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
}

void AMultiplayerGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiplayerGameMode::SpawnCharactersToPlayerStarts()
{
	if(PlayerStarts.Num() == 0)
    {
    UE_LOG(LogTemp, Warning, TEXT("No PlayerStarts found!"));
    return;
    }
    
    for(int32 i = 0; i < ConnectedControllers.Num(); i++)
    {
	    if(i >= PlayerStarts.Num())
	    {
	    	UE_LOG(LogTemp, Warning, TEXT("Not enough PlayerStarts for all players!"));
	    	return;
	    }
    
    	APawn* Character = GetWorld()->SpawnActor<APawn>(CharacterClass, PlayerStarts[i]->GetActorLocation(), PlayerStarts[i]->GetActorRotation());
    
    	if(auto Controller = UGameplayStatics::GetPlayerController(GetWorld(), i))
    	{
    		Controller->bAutoManageActiveCameraTarget = false;
    		Controller->Possess(Character);
    	}else
    	{
    		APlayerController* PlayerController = UGameplayStatics::CreatePlayer(GetWorld(), i, true);
    		PlayerController->bAutoManageActiveCameraTarget = false;
    		PlayerController->Possess(Character);
    	}
    
    	if(Character != nullptr){
    		Character->Rename(*FString::Printf(TEXT("Joueur %d"), i+1));
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

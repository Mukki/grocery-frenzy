// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "MultiplayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GROCERYFRENZY_API AMultiplayerGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	public:
    AMultiplayerGameMode();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<AActor*> Characters;
	
	private:
    void CollectAllPlayerStarts();
    void SpawnCharactersToPlayerStarts();

	UPROPERTY()
    TArray<APlayerStart*> PlayerStarts;
    
    UPROPERTY()
    TArray<FInputDeviceId> ConnectedControllers;
    
    //TODO: Edit to use our own character class
    UPROPERTY(EditAnywhere, Category = "Bomberman settings")
    TSubclassOf<APawn> CharacterClass;
};

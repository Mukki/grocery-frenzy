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

	UFUNCTION(BlueprintCallable, Category = "MultiplayerGameMode")
	void StartGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> Characters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameDurationInSeconds = 180.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Timer;


private:
	void CollectAllPlayerStarts();
	void SpawnCharactersToPlayerStarts();

	UPROPERTY()
	TArray<APlayerStart*> PlayerStarts;

	UPROPERTY()
	TArray<FInputDeviceId> ConnectedControllers;

	UPROPERTY()
	bool GameStarted = false;	
	UPROPERTY()
	bool GameFinished = false;

	//TODO: Edit to use our own character class
	UPROPERTY(EditAnywhere, Category = "Bomberman settings")
	TSubclassOf<APawn> CharacterClass;
};

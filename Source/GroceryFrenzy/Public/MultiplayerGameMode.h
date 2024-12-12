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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool GameStarted = false;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool GameFinished = false;


private:
	void CollectAllPlayerStarts();
	void SetInputEnabled(const bool bEnableInput);
	void SpawnCharactersToPlayerStarts();
	void OnGameEnd();
	
	UPROPERTY()
	TArray<APlayerStart*> PlayerStarts;

	UPROPERTY()
	TArray<FInputDeviceId> ConnectedControllers;
	
	UPROPERTY()
	FTimerHandle InitTimer;
	
	UPROPERTY(EditAnywhere, Category = "MultiplayerGameMode")
	TSubclassOf<APawn> CharacterClass1;
	UPROPERTY(EditAnywhere, Category = "MultiplayerGameMode")
	TSubclassOf<APawn> CharacterClass2;
};

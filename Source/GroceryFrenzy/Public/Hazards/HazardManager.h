// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HazardManager.generated.h"

UCLASS()
class GROCERYFRENZY_API AHazardManager : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDelayBetweenHazard = 5.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayDecrease = 2.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDelayBetweenHazard = 1.0;

	// Sets default values for this actor's properties
	AHazardManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Triggers a randomly choosen hazard in the level
	UFUNCTION()
	void TriggerRandomHazard();

	UPROPERTY()
	float CurrentDelayBetweenHazard;
	UPROPERTY()
	FTimerHandle HazardTimerHandle;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

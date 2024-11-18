// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazards/HazardManager.h"

#include "MaterialHLSLTree.h"
#include "Hazards/AHazard.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHazardManager::AHazardManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AHazardManager::BeginPlay()
{
	Super::BeginPlay();
	CurrentDelayBetweenHazard = BaseDelayBetweenHazard;
	// Starts a loop that will trigger hazards"
	GetWorld()->GetTimerManager().SetTimer(
		HazardTimerHandle,
		this,
		&AHazardManager::TriggerRandomHazard,
		CurrentDelayBetweenHazard,
		true);
}

void AHazardManager::TriggerRandomHazard()
{
	// Gets the list of inactive Hazards in the world
	TArray<AActor*> AllHazards;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHazard::StaticClass(), AllHazards);
	TArray<AActor*> TriggerableHazards = AllHazards.FilterByPredicate([](const AActor* Actor)
	{
		const AHazard* Hazard = Cast<AHazard>(Actor);
		return Hazard->IsActive == false;
	});

	// Picks a random hazard an triggers it
	if (TriggerableHazards.Num() > 0)
	{
		const int Index = FMath::RandRange(0, TriggerableHazards.Num() - 1);
		AHazard* Hazard = Cast<AHazard>(TriggerableHazards[Index]);
		Hazard->Trigger();

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hazard triggered"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("No hazard found"));
	}

	// Calculates the delay before the next call
	CurrentDelayBetweenHazard -= DelayDecrease;
	if (CurrentDelayBetweenHazard <= MinDelayBetweenHazard)
	{
		CurrentDelayBetweenHazard = MinDelayBetweenHazard;
	}
	
	// Update the timer with the new delay
	GetWorldTimerManager().ClearTimer(HazardTimerHandle);
	GetWorldTimerManager().SetTimer(
		HazardTimerHandle,
		this,
		&AHazardManager::TriggerRandomHazard,
		CurrentDelayBetweenHazard,
		true);
}

// Called every frame
void AHazardManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

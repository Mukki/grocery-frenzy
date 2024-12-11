// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AHazard.h"
#include "GameFramework/Actor.h"
#include "WaterLeak.generated.h"

class UBoxComponent;

UCLASS()
class GROCERYFRENZY_API AWaterLeak : public AHazard
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaterLeak();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* OverlapBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* LeakRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Raycaster;

	UPROPERTY()
	USceneComponent* BlueprintRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterLeak")
	float SpreadingSpeed = .3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterLeak")
	float WaterCurrentForce = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterLeak")
	float MaxLeakLength = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterLeak")
	float LeakDuration = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterLeak")
	float CooldownDuration = 5.0f;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                int32 OtherBodyIndex);

protected:
	UPROPERTY()
	int SpreadFactor = 0;
	UPROPERTY()
	FVector StartLocation;
	UPROPERTY()
	FVector ForwardVector = FVector::Zero();
	UPROPERTY()
	TArray<AActor*> OverlappingActors = {};
	UPROPERTY()
	FTimerHandle DissipationTimer;
	UPROPERTY()
	FTimerHandle CooldownTimer;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Trigger() override;
	void Dissipate();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	bool CheckForVoidUnderneath(const FVector& Position);
};

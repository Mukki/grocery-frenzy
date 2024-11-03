// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AHazard.h"
#include "GameFramework/Actor.h"
#include "FallingTile.generated.h"

class UBoxComponent;

UCLASS()
class GROCERYFRENZY_API AFallingTile : public AHazard
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallingTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Trigger() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Collider;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

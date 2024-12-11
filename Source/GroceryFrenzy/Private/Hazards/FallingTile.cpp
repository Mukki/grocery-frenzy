// Fill out your copyright notice in the Description page of Project Settings.

#include "Hazards/FallingTile.h"
#include "Components/BoxComponent.h"

// Sets default values
AFallingTile::AFallingTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	StaticMesh->SetupAttachment(Collider);
}

// Called when the game starts or when spawned
void AFallingTile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFallingTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFallingTile::Trigger()
{
	Super::Trigger();
	Collider->SetEnableGravity(true);
	Collider->SetSimulatePhysics(true);
	IsActive = true;
}



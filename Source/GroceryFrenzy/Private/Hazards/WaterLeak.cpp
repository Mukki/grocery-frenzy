// Fill out your copyright notice in the Description page of Project Settings.

#include "Hazards/WaterLeak.h"

#include "Components/BoxComponent.h"


// Sets default values
AWaterLeak::AWaterLeak()
{
	PrimaryActorTick.bCanEverTick = true;

	// Initialisation des composants
	BlueprintRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	LeakRoot = CreateDefaultSubobject<USceneComponent>("Origin");
	OverlapBox = CreateDefaultSubobject<UBoxComponent>("Overlap Box");
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	Raycaster = CreateDefaultSubobject<USceneComponent>("Raycaster");

	RootComponent = BlueprintRoot;
	LeakRoot->SetupAttachment(BlueprintRoot);
	OverlapBox->SetupAttachment(LeakRoot);
	StaticMesh->SetupAttachment(OverlapBox);
	Raycaster->SetupAttachment(OverlapBox);

	// Configuration boite de détection des acteurs
	OverlapBox->SetCollisionProfileName("OverlapAllDynamic");
	OverlapBox->SetGenerateOverlapEvents(true);
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AWaterLeak::BeginOverlap);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AWaterLeak::EndOverlap);
	StaticMesh->SetCollisionProfileName("NoCollision");

	// Définit l'échelle sur X à 0 par défaut
	LeakRoot->SetRelativeScale3D(OverlapBox->GetRelativeScale3D() * FVector(0, 1, 1));
}

// Called when the game starts or when spawned
void AWaterLeak::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	ForwardVector = GetActorRotation().Vector();
	SetActorTickEnabled(false); // désactive le tick tant que la fuite n'est pas triggered
}

void AWaterLeak::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	if (OtherActor && !OverlappingActors.Contains(OtherActor))
	{
		OverlappingActors.Add(OtherActor);
	}
}


void AWaterLeak::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingActors.Remove(OtherActor);
}


void AWaterLeak::Trigger()
{
	IsActive = true;
	SpreadFactor = 1.0f;
	SetActorTickEnabled(true);
}


void AWaterLeak::Dissipate()
{
	SpreadFactor = -1.0f;
	SetActorTickEnabled(true);
}

// Called every frame
void AWaterLeak::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActive && OverlappingActors.Num() > 0)
	{
		FVector const LocationIncrement = WaterCurrentForce * ForwardVector * DeltaTime;
		int i = 0;
		for (auto const Actor : OverlappingActors)
		{
			Actor->SetActorLocation(Actor->GetActorLocation() + LocationIncrement);
			i++;
		}
	}

	if (SpreadFactor > 0)
	{
		LeakRoot->SetRelativeScale3D(LeakRoot->GetRelativeScale3D() + FVector(SpreadingSpeed * SpreadFactor, 0, 0));
		// Arrête l'expansion de la fuite si atteint le vide
		if (CheckForVoidUnderneath(Raycaster->GetComponentLocation()))
		{
			SpreadFactor = 0;
			// garantit que la fuite dépasse bien dans le vide
			LeakRoot->SetRelativeScale3D(LeakRoot->GetRelativeScale3D() + FVector(.4, 0, 0));

			// Créé un timer pour dissiper la fuite
			GetWorldTimerManager().ClearTimer(DissipationTimer);
			GetWorldTimerManager().SetTimer(DissipationTimer, this, &AWaterLeak::Dissipate, LeakDuration);
		}
	}
	else if (SpreadFactor < 0)
	{
		LeakRoot->SetRelativeScale3D(LeakRoot->GetRelativeScale3D() + FVector(SpreadingSpeed * SpreadFactor, 0, 0));

		if (LeakRoot->GetRelativeScale3D().X < 0.01)
		{
			IsActive = false;
			SpreadFactor = 0;
			LeakRoot->SetRelativeScale3D(FVector(0, 1, 1));
			SetActorTickEnabled(false);
		}
	}
}

bool AWaterLeak::CheckForVoidUnderneath(const FVector& Position)
{
	// Distance pour le raycast
	const float RaycastDistance = 1000.0f;

	FVector Start1 = Position;
	FVector End1 = Start1 - FVector(0.0f, 0.0f, RaycastDistance);

	// Paramètres du raycast
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true; // Utilisation de collisions complexes

	// Résultats des raycasts
	FHitResult HitResult1;
	bool bHit1 = GetWorld()->LineTraceSingleByChannel(HitResult1, Start1, End1, ECC_Visibility, TraceParams);

	// Debug : dessiner les raycasts
	FColor Color1 = bHit1 ? FColor::Red : FColor::Green;
	DrawDebugLine(GetWorld(), Start1, End1, Color1, false, 1.0f);

	return !bHit1;
}

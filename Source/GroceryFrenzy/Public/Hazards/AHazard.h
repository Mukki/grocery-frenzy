#pragma once
#include "AHazard.generated.h"

UCLASS()
class AHazard : public AActor
{
	GENERATED_BODY()
	
public:
	// True when the hazard's effects are currently running
	UPROPERTY()
	bool IsActive = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	USoundBase* TriggerSound;

	// Function to call to make the hazard active
	UFUNCTION()
	virtual void Trigger();
};

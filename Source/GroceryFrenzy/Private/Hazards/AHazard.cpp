#include "Hazards/AHazard.h"

#include "Kismet/GameplayStatics.h"

void AHazard::Trigger()
{
	if (TriggerSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, TriggerSound, GetActorLocation());
	}
}

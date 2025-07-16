#include "DroneGameMode.h"
#include "DronePawn.h"

ADroneGameMode::ADroneGameMode()
{
	DefaultPawnClass = ADronePawn::StaticClass();
}


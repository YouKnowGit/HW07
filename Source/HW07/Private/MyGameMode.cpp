#include "MyGameMode.h"
#include "MyPlayerPawn.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = AMyPlayerPawn::StaticClass();
}

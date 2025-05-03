// LeaveMeAlone Game by Netologiya. All RightsReserved


#include "LMAGameMode.h"

#include "LMADefaultCharacter.h"
#include "LMAPlayerController.h"

ALMAGameMode::ALMAGameMode()
{
	DefaultPawnClass = ALMADefaultCharacter::StaticClass();
	PlayerControllerClass = ALMAPlayerController::StaticClass();
}

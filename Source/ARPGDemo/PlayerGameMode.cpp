#include "PlayerGameMode.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"
#include "UObject/ConstructorHelpers.h"

APlayerGameMode::APlayerGameMode()
{
	PlayerControllerClass = APlayerCharacterController::StaticClass();

}

void APlayerGameMode::Win()
{
	UE_LOG(LogTemp, Warning, TEXT("WIN!"));

}

void APlayerGameMode::Loss()
{
	UE_LOG(LogTemp, Warning, TEXT("LOSE!"));

}
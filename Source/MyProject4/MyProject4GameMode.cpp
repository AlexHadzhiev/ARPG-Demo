// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject4GameMode.h"
#include "MyProject4PlayerController.h"
#include "MyProject4Character.h"
#include "UObject/ConstructorHelpers.h"

AMyProject4GameMode::AMyProject4GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMyProject4PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
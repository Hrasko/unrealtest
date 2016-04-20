// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TopDownShips.h"
#include "TopDownShipsGameMode.h"
#include "TopDownShipsPlayerController.h"
#include "TopDownShipsCharacter.h"

ATopDownShipsGameMode::ATopDownShipsGameMode()
{
	/*
	// use our custom PlayerController class
	PlayerControllerClass = ATopDownShipsPlayerController::StaticClass();
	*/
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyShipPawn"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
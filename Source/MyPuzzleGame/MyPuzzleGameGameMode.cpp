// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyPuzzleGameGameMode.h"
#include "MyPuzzleGameHUD.h"
#include "MyPuzzleGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyPuzzleGameGameMode::AMyPuzzleGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMyPuzzleGameHUD::StaticClass();
}

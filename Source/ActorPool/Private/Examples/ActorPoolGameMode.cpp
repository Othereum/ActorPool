// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ActorPoolGameMode.h"
#include "ActorPoolHUD.h"
#include "ActorPoolCharacter.h"
#include "UObject/ConstructorHelpers.h"

AActorPoolGameMode::AActorPoolGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AActorPoolHUD::StaticClass();
}

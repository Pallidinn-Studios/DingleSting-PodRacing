// Copyright Epic Games, Inc. All Rights Reserved.

#include "PodRacingGameMode.h"
#include "PodRacingCharacter.h"
#include "UObject/ConstructorHelpers.h"

APodRacingGameMode::APodRacingGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

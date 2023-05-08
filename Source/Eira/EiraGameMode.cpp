// Copyright Epic Games, Inc. All Rights Reserved.

#include "EiraGameMode.h"
#include "EiraCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEiraGameMode::AEiraGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

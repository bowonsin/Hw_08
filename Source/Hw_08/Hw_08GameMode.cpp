// Copyright Epic Games, Inc. All Rights Reserved.

#include "Hw_08GameMode.h"
#include "Hw_08Character.h"
#include "UObject/ConstructorHelpers.h"

AHw_08GameMode::AHw_08GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

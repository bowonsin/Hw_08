// Copyright Epic Games, Inc. All Rights Reserved.

#include "HW_08GameMode.h"
#include "HW_08Character.h"
#include "UObject/ConstructorHelpers.h"

AHW_08GameMode::AHW_08GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

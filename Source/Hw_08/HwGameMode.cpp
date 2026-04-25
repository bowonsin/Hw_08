// Fill out your copyright notice in the Description page of Project Settings.


#include "HwGameMode.h"
#include "HwCharacter.h"
#include "HwPlayerController.h"

AHwGameMode::AHwGameMode()
{
	DefaultPawnClass = AHwCharacter::StaticClass();
	PlayerControllerClass = AHwPlayerController::StaticClass();

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "HwGameInstance.h"

UHwGameInstance::UHwGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UHwGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	//UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}

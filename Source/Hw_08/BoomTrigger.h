// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoomTrigger.generated.h"

UCLASS()
class HW_08_API ABoomTrigger : public AActor
{
	GENERATED_BODY()
	
public:	

	ABoomTrigger();

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerBox;

	FVector GetRandomPotoint()const;

};

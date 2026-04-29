// Fill out your copyright notice in the Description page of Project Settings.


#include "PushingItem01.h"
#include "Components/SphereComponent.h"
#include "HwCharacter.h"

APushingItem01::APushingItem01()
{
	PrimaryActorTick.bCanEverTick = true;

	PushRadius = 150.0f;
	Collision->InitSphereRadius(PushRadius);
	
	f_Speed_x = FMath::RandRange(150.0f, 500.0f);
	f_Direction = 1.0f;
	MaxRange = 250.0f;
	
}

void APushingItem01::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

}

void APushingItem01::ActivateItem(AActor* Activator)
{
	// 태그시 플레이어 케릭터인지 확인
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AHwCharacter* PlayerCharacter = Cast<AHwCharacter>(Activator))
		{
			// Character 밀리는용으로 작성
			FVector Forward = PlayerCharacter->GetActorForwardVector();

			// 진행 반대 방향
			FVector LaunchDir = -Forward;

			// 힘 조절 (수평 + 약간 위로)
			FVector LaunchVelocity = LaunchDir * 800.0f + FVector(0, 0, 300.0f);
			PlayerCharacter->LaunchCharacter(LaunchVelocity, true, true);
		}
	}
}

void APushingItem01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(f_Speed_x))
	{
		// 이동
		AddActorLocalOffset(FVector(f_Direction * f_Speed_x * DeltaTime, 0, 0));

		// 거리 계산 (시작 위치 기준) 
		float Distance = GetActorLocation().X - StartLocation.X;

		// 범위 초과 시 방향 반전
		if (Distance >= MaxRange || Distance <= -MaxRange)
		{
			f_Direction *= -1;
		}
	}
}



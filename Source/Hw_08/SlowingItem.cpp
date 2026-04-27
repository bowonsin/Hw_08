// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowingItem.h"
#include "HwCharacter.h"
#include "Components/SphereComponent.h"
ASlowingItem::ASlowingItem()
{
	DownSpeed = 0.1f;
	duration = 5.0f;
	SlowRadius = 250.0f;

	SlowCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	SlowCollision->InitSphereRadius(SlowRadius);
	SlowCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SlowCollision->SetupAttachment(Scene);
}

void ASlowingItem::ActivateItem(AActor* Activator)
{
	// 태그시 플레이어 케릭터인지 확인
	if (Activator && Activator->ActorHasTag("Player"))
	{
		// 회복 디버그 메시지
		if (AHwCharacter* PlayerCharacter = Cast<AHwCharacter>(Activator))
		{
			GetWorldTimerManager().ClearTimer(ItemTimerHandle);


			PlayerCharacter->SetDecreaseSpeed(DownSpeed);
			// 해당 Actor의 속도가 느려지고 일정 시간 뒤에 속도가 원상태로 돌아가게 설정
			GetWorldTimerManager().SetTimer(
				ItemTimerHandle,
				PlayerCharacter,
				&AHwCharacter::ResetSpeed,
				duration,
				false
			);
		}
		DestroyItem();
	}
}

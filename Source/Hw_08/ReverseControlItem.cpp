// Fill out your copyright notice in the Description page of Project Settings.


#include "ReverseControlItem.h"
#include "HwCharacter.h"
#include "Components/SphereComponent.h"

AReverseControlItem::AReverseControlItem()
{
	duration = 5.0f;
	ReverseRadius = 250.0f;

	ReverseCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ReverseCollision->InitSphereRadius(ReverseRadius);
	ReverseCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ReverseCollision->SetupAttachment(Scene);
}

void AReverseControlItem::ActivateItem(AActor* Activator)
{
	// 태그시 플레이어 케릭터인지 확인
	if (Activator && Activator->ActorHasTag("Player"))
	{
		// 회복 디버그 메시지
		if (AHwCharacter* PlayerCharacter = Cast<AHwCharacter>(Activator))
		{
			GetWorldTimerManager().ClearTimer(ReverseItemTimerHandle);

			// 
			PlayerCharacter->GetReverseMoving();
			// 해당 Actor의 속도가 느려지고 일정 시간 뒤에 속도가 원상태로 돌아가게 설정
			GetWorldTimerManager().SetTimer(
				ReverseItemTimerHandle,
				PlayerCharacter,
				&AHwCharacter::ReSetReverseMoving,
				duration,
				false
			);
		}
		DestroyItem();
	}
}

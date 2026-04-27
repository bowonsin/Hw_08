// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"
#include "HwCharacter.h"
#include "HwGameState.h"
AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        // 회복 디버그 메시지
		if(AHwCharacter* PlayerCharacter = Cast<AHwCharacter>(Activator))
		{
			// 캐릭터의 체력을 회복
			PlayerCharacter->AddHealth(HealAmount);
		}
        DestroyItem();
    }

}

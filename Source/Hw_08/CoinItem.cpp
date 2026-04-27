// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"
#include "HwGameState.h"

ACoinItem::ACoinItem()
{
    // 점수 기본값을 0으로 설정
    PointValue = 10;
    ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
    // 플레이어 태그 확인
    if (Activator && Activator->ActorHasTag("Player"))
    {
        // 점수 획득 디버그 메시지
        if (UWorld* World = GetWorld())
        {
            if (AHwGameState* GameState = World->GetGameState<AHwGameState>())
            {
                // score 추가
                GameState->AddScore(PointValue);

                // Coin 모두 줍기
                GameState->OnCoinCollected();
            }
        }
        // 부모 클래스 (BaseItem)에 정의된 아이템 파괴 함수 호출
        DestroyItem();
    }
}

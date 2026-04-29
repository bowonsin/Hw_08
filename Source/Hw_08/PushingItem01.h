// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "PushingItem01.generated.h"

/**
 * 
 */
UCLASS()
class HW_08_API APushingItem01 : public ABaseItem
{
	GENERATED_BODY()
	
public:
	APushingItem01();
	// Coliision 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
	float PushRadius;

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	
	// 부모 클래스에서 상속받은 ActivateItem 함수를 오버라이드
	virtual void ActivateItem(AActor* Activator) override;

protected:
	// 시작 위치 저장
	FVector StartLocation;

	// 이동 속도 
	float f_Speed_x;
	// 방향
	float f_Direction;
	// 이동 거리
	float MaxRange;
};

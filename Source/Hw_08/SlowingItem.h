// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SlowingItem.generated.h"

/**
 * 
 */
UCLASS()
class HW_08_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()
public:

	ASlowingItem();

protected:
	// Collision 딴거없음
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* SlowCollision;

	// Coliision 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float SlowRadius;

	// 데미지 속도 
	FTimerHandle ItemTimerHandle;

	// 저하 속도
	float DownSpeed;
	// 지속시간
	float duration;

	virtual void ActivateItem(AActor* Activator) override;
};

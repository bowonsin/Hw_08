// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ReverseControlItem.generated.h"

/**
 * 
 */
UCLASS()
class HW_08_API AReverseControlItem : public ABaseItem
{
	GENERATED_BODY()
public:
	AReverseControlItem();
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* ReverseCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	float ReverseRadius;

	// 데미지 속도 
	FTimerHandle ReverseItemTimerHandle;

	// 지속시간
	float duration;

	virtual void ActivateItem(AActor* Activator) override;
	
};

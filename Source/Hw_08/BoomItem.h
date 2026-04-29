// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BoomItem.generated.h"

/**
 * 
 */
UCLASS()
class HW_08_API ABoomItem : public ABaseItem
{
	GENERATED_BODY()
public:
	ABoomItem();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* ExplosionCollision;

	// 폭발 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boom")
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boom")
	float BoomDamage;

	virtual void Tick(float DeltaTime) override;


	// 부모 클래스에서 상속받은 ActivateItem 함수를 오버라이드
	virtual void ActivateItem(AActor* Activator) override;

protected:
	void MoveGravity(float DeltaTime);

	void BoomAttack();
};

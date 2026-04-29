// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomItem.h"
#include "HwCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BoomTrigger.h"


ABoomItem::ABoomItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ExplosionRadius = 500.0f;
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
	ExplosionCollision->SetGenerateOverlapEvents(true);
	Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);

	
	BoomDamage = 15.0f;
}

void ABoomItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveGravity(DeltaTime);
}

void ABoomItem::ActivateItem(AActor* Activator)
{
	// Trigger 터지는거 허..
	if (Activator->IsA(ABoomTrigger::StaticClass()))
	{
		BoomAttack();
		Destroy();
	}
}

void ABoomItem::MoveGravity(float DeltaTime)
{
	// 하강 판정 .
	FVector Velocity(0, 0, 0);
	// 1. 중력 가속도 적용 (Z축)
	Velocity.Z += -980.f * DeltaTime *30.0f;
	// 2. 위치 이동 //월드 값으로 하강해야지 현제 상태에서 Z값으로 내려가기 때문에 
	// ( 이동 되는 좌표 값 , bsweep 체크 ( 벽이나 오브젝트에 부딫히면 멈추거나 밀리지않음?.. 뚷고 가던데?..) 
	AddActorWorldOffset(Velocity * DeltaTime, true);
}

void ABoomItem::BoomAttack()
{
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Name : %s") , *Actor->GetName());

		if (Actor && Actor->ActorHasTag("Player"))
		{
			// 데미지를 발생시켜 Actor->TakeDamage()가 실행되도록 함

			UGameplayStatics::ApplyDamage(
				Actor,                      // 데미지를 받을 액터
				BoomDamage,            // 데미지 양
				nullptr,                    // 데미지를 유발한 주체 (지뢰를 설치한 캐릭터가 없으므로 nullptr)
				this,                       // 데미지를 유발한 오브젝트(지뢰)
				UDamageType::StaticClass()  // 기본 데미지 유형
			);
		}
	}
}

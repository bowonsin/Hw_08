// Fill out your copyright notice in the Description page of Project Settings.


#include "HwGameState.h"
#include "HwGameInstance.h"
#include "HwPlayerController.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Kismet//GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint//UserWidget.h"

AHwGameState::AHwGameState()
{
	
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 10.0f; // 한 레벨당 30초
	CurrentLevelIndex = 0;
	MaxLevels = 3;

	StageTimer.Add(60.0f);
	StageTimer.Add(45.0f);
	StageTimer.Add(30.0f);

	CountStage = 0;
	MaxStage = 3;
	CountSpawn = 0;
}

void AHwGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AHwGameState::UpdateHUD,
		0.1f,
		true
	);
}
int32 AHwGameState::GetScore() const
{
	return Score;
}
void AHwGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		
		UHwGameInstance* HwGameInstance = Cast<UHwGameInstance>(GameInstance);
		if (HwGameInstance)
		{
			HwGameInstance->AddToScore(Amount);
		}
	}
}
void AHwGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AHwPlayerController* HwPlayerController = Cast<AHwPlayerController>(PlayerController))
		{
			HwPlayerController->SetPause(true);
			HwPlayerController->ShowMainMenu(true);
		}
	}
	// 여기서 UI를 띄운다거나, 재시작 기능을 넣을 수도 있음
}
void AHwGameState::SpawnActor()
{
	ResetActor();
	// 현재 맵에 배치된 모든 SpawnVolume을 찾아 아이템 40개를 스폰
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	const int32 ItemToSpawn = 30;
	SpawnedCoinCount = ItemToSpawn;
	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();

				SpawnedActors.Add(SpawnedActor);

				// 만약 스폰된 액터가 코인 타입이라면 SpawnedCoinCount 증가
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}
}
void AHwGameState::ResetActor()
{
	// 1. 기존 액터 제거
	for (AActor* Actor : SpawnedActors)
	{
		ABaseItem* Item = Cast<ABaseItem>(Actor);
		if (IsValid(Item))
		{
			Item->DestroyItem();
		}
	}
	// TArray<AActor> 비우고 
	SpawnedActors.Empty();
	// CoinCount는 다시 0으로 초기화 
	//SpawnedCoinCount = 0;
}
void AHwGameState::StartLevel()
{
	// 시작 HUD 설정
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AHwPlayerController* HwPlayerController = Cast<AHwPlayerController>(PlayerController))
		{
			HwPlayerController->ShowGameHUD();
		}
	}

	//  [추가] GameInstance에서 현재 레벨 값 가져오기 (Sparta에 있던 핵심 로직)
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHwGameInstance* HwGameInstance = Cast<UHwGameInstance>(GameInstance);
		if (HwGameInstance)
		{
			CurrentLevelIndex = HwGameInstance->CurrentLevelIndex; // 🔥 추가
		}
	}
	// 레벨 시작 시, 코인 개수 초기화
	StartStage();
}
void AHwGameState::StartStage()
{
	// Stage 시작하면 
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	CountSpawn = CountStage - MaxStage;
	
	// Actor 스폰 
	SpawnManage();

	// StageTimerHandle로 사용하고 
	// 시간이 끝나면 EndStage 함수 호출
	// 시간은 TArray 에 저장된 값들 기준으로 
	GetWorldTimerManager().SetTimer(
		StageTimerHandle,
		this,
		&AHwGameState::EndStage,
		StageTimer[CountStage],
		false
	);
	// Ui Update
	UpdateHUD();
}
void AHwGameState::EndStage()
{
	GetWorldTimerManager().ClearTimer(StageTimerHandle);

	// 일단 Stage 에 있는 Actor들을 제거 
	ResetActor();
	CountStage++;
	// 현제 스테이
	if (CountStage < MaxStage)
	{
		// 최대 Stage를 채우지 못했다면 다시 StartStage로 
		StartStage();
	}
	else
	{
		CountStage = 0;
		EndLevel();
	}
}
void AHwGameState::OnLevelTimeUp()
{
	// 시간이 다 되면 레벨을 종료
	EndLevel();
}
void AHwGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	// 현재 레벨에서 스폰된 코인을 전부 주웠다면 즉시 레벨 종료
	// 현제 스테이지에 따라서 스테이지 종료
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndStage();
	}
}
void AHwGameState::EndLevel()
{
	//GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHwGameInstance* HwGameInstance = Cast<UHwGameInstance>(GameInstance);
		if (HwGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			HwGameInstance->CurrentLevelIndex = CurrentLevelIndex;

			if (CurrentLevelIndex >= MaxLevels)
			{
				OnGameOver();
				return;
			}

			if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
			{
				UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
			}
			else
			{
				OnGameOver();
			}
		}
	}
}
void AHwGameState::SpawnManage()
{
	if (CountSpawn != 0)
	{
		// 사용하기 전에 ClearTimer 사용해서 쓰기 쉽게 만들어 놓기
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		// 스폰하기 전에 다시 한번더 확인하는 형식으로 호출
		ResetActor();
		SpawnActor(); // 스폰 
		// Stage 별로 20 초 
		GetWorldTimerManager().SetTimer(
			SpawnTimerHandle,
			this,
			&AHwGameState::SpawnManage,
			20.0f,
			false
		);
		CountSpawn--;
	}
	//횟수 만큼 
}
void AHwGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AHwPlayerController* HwPlayerController = Cast<AHwPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = HwPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					//float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					// Level 에서 Stage 형식으로 바뀌었기 때문에 Stage로 변경
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(StageTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UHwGameInstance* HwGameInstance = Cast< UHwGameInstance>(GameInstance);
						if (HwGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), HwGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d / Stage : % d"), CurrentLevelIndex + 1 , CountStage + 1)));
				}
			}
		}
	}
}

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
			UE_LOG(LogTemp, Warning, TEXT("Score Added: %d (Total: %d)"),
				Amount,
				HwGameInstance->TotalScore);
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

void AHwGameState::StartLevel()
{
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
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	// 현재 맵에 배치된 모든 SpawnVolume을 찾아 아이템 40개를 스폰
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				// 만약 스폰된 액터가 코인 타입이라면 SpawnedCoinCount 증가
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	// 30초 후에 OnLevelTimeUp()가 호출되도록 타이머 설정
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AHwGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);
	UpdateHUD();

	UE_LOG(LogTemp, Warning, TEXT("Level %d Start!, Spawned %d coin"),
		CurrentLevelIndex + 1,
		SpawnedCoinCount);
}

void AHwGameState::OnLevelTimeUp()
{
	// 시간이 다 되면 레벨을 종료
	EndLevel();
}

void AHwGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	UE_LOG(LogTemp, Warning, TEXT("Coin Count: %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount);

		// 현재 레벨에서 스폰된 코인을 전부 주웠다면 즉시 레벨 종료
		if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
		{
			EndLevel();
		}
}

void AHwGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

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

void AHwGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		
		AHwPlayerController* HwPlayerController = Cast<AHwPlayerController>(PlayerController);
		{
			if (UUserWidget* HUDWidget = HwPlayerController->GetHUDWidget())
			{
				
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
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
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}
			}
		}
	}
}

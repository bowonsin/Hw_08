// Fill out your copyright notice in the Description page of Project Settings.


#include "HwPlayerController.h"
#include "HwGameState.h"
#include "HwGameInstance.h"
#include "EnhancedInputSubsystems.h" // Enhanced Input System의 Local Player Subsystem을 사용하기 위해 포함
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h" // 헤더 추가
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

AHwPlayerController::AHwPlayerController()
	:
	InputMappingContext(nullptr)
	,MoveAction(nullptr)
	,JumpAction(nullptr)
	,LookAction(nullptr)
	,SprintAction(nullptr)
	,HUDWidgetClass(nullptr)
	,HUDWidgetInstance(nullptr)
	,MainMenuWidgetClass(nullptr)
	,MainMenuWidgetInstance(nullptr)
{
}


void AHwPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 현재 PlayerController에 연결된 Local Player 객체를 가져옴    
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		// Local Player에서 EnhancedInputLocalPlayerSubsystem을 획득
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				// Subsystem을 통해 우리가 할당한 IMC를 활성화
				// 우선순위(Priority)는 0이 가장 높은 우선순위
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
	// 게임 실행 시 메뉴 레벨에서 메뉴 UI 먼저 표시
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
	else
	{
		ShowGameHUD(); //  안전장치
	}
	/*

	// HUD 위젯 생성 및 표시
	if (HUDWidgetClass)
	{
		UUserWidget* HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		//HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
	*/
	AHwGameState* HwGameState = GetWorld() ? GetWorld()->GetGameState<AHwGameState>() : nullptr;
	if (HwGameState)
	{
		HwGameState->UpdateHUD();
	}
}

UUserWidget* AHwPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void AHwPlayerController::ShowGameHUD()
{
	// HUD가 켜져 있다면 닫기
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	// 이미 메뉴가 떠 있으면 제거
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());

			AHwGameState* HwGameState = GetWorld() ? GetWorld()->GetGameState<AHwGameState>() : nullptr;
			if (HwGameState)
			{
				HwGameState->UpdateHUD();
			}
		}
	}
}

void AHwPlayerController::ShowMainMenu(bool bIsRestart)
{
	// HUD가 켜져 있다면 닫기
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	// 이미 메뉴가 떠 있으면 제거
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	// 메뉴 UI 생성
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart")));
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("Start")));
			}
		}

		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (UHwGameInstance* SpartaGameInstance = Cast<UHwGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Total Score: %d"), SpartaGameInstance->TotalScore)
					));
				}
			}
		}
	}
}

void AHwPlayerController::StartGame()
{
	if (UHwGameInstance* HwGameInstance = Cast<UHwGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		HwGameInstance->CurrentLevelIndex = 0;
		HwGameInstance->TotalScore = 0;
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);
}


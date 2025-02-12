#include "Num6PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Num6GameState.h"
#include "Components/TextBlock.h"
#include "Num6GameInstance.h"
#include "Kismet/GameplayStatics.h"


ANum6PlayerController::ANum6PlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	JumpAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr)
{
}

void ANum6PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 1);
			}
		}
	}
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}

}

UUserWidget* ANum6PlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ANum6PlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromViewport();
		HUDWidgetInstance = nullptr;
	}
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromViewport();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			// UI가 입력을 가로채지 않도록 설정
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}

		ANum6GameState* Num6GameState = GetWorld() ? GetWorld()->GetGameState<ANum6GameState>() : nullptr;
		if (Num6GameState)
		{
			Num6GameState->UpdateHud();
		}
	}
}


void ANum6PlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromViewport();
		HUDWidgetInstance = nullptr;
	}
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromViewport();
		MainMenuWidgetInstance = nullptr;
	}

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
				if (UNum6GameInstance* Num6GameInstance = Cast<UNum6GameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Total Score: %d"), Num6GameInstance->TotalScore)
					));
				}
			}
		}

	}
}

void ANum6PlayerController::StartGame()
{
	if (UNum6GameInstance* Num6GameInstance = Cast<UNum6GameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		Num6GameInstance->CurrentLevelIndex = 0;
		Num6GameInstance->TotalScore = 0;
	}

	// 게임 입력 모드로 변경
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());

	UGameplayStatics::OpenLevel(GetWorld(), FName("Stage1"));
	SetPause(false);
}


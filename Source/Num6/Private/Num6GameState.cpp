// Fill out your copyright notice in the Description page of Project Settings.


#include "Num6GameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "Num6GameInstance.h"
#include "CoinItem.h"
#include "Num6PlayerController.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
ANum6GameState::ANum6GameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void ANum6GameState::BeginPlay()
{
	Super::BeginPlay();


	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ANum6GameState::UpdateHud,
		0.1f,
		true
	);
}

int32 ANum6GameState::GetScore() const
{
	return Score;
}

void ANum6GameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UNum6GameInstance* Num6GameInstance = Cast<UNum6GameInstance>(GameInstance);
		if (Num6GameInstance)
		{
			Num6GameInstance->AddToScore(Amount);
		}
	}

}

void ANum6GameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ANum6PlayerController* Num6PlayerController = Cast<ANum6PlayerController>(PlayerController))
		{
			Num6PlayerController->ShowGameHUD();
		}
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 TotalItemsToSpawn = 20;
	const int32 NumVolumes = FoundVolumes.Num();

	if (NumVolumes > 0)
	{
		int32 BaseSpawnCount = TotalItemsToSpawn / NumVolumes;
		int32 ExtraItems = TotalItemsToSpawn % NumVolumes;
		int32 SpawnedItems = 0;

		for (int32 i = 0; i < NumVolumes; i++)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[i]);
			if (SpawnVolume)
			{
				int32 NumToSpawn = BaseSpawnCount + (i < ExtraItems ? 1 : 0);
				for (int32 j = 0; j < NumToSpawn; j++)
				{
					AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
					if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
					{
						SpawnedCoinCount++;
					}
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ANum6GameState::OnLevelTimeUp,
		LevelDuration,
		false
	);

}


void ANum6GameState::OnLevelTimeUp()
{
	EndLevel();

}

void ANum6GameState::OnCoinCollected()
{
	CollectedCoinCount++;

	// 🔍 현재 코인 개수 확인
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"), CollectedCoinCount, SpawnedCoinCount);

	// ✅ SpawnedCoinCount가 올바르게 설정되었는지 확인
	if (SpawnedCoinCount <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("⚠️ SpawnedCoinCount가 0 이하임! 확인 필요!"));
	}

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}


void ANum6GameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UNum6GameInstance* Num6GameInstance = Cast<UNum6GameInstance>(GameInstance);
		if (Num6GameInstance)
		{
			AddScore(Score);
			Num6GameInstance->CurrentLevelIndex++; // 게임 인스턴스에서도 증가
			CurrentLevelIndex = Num6GameInstance->CurrentLevelIndex;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Moving to Next Level: %d"), CurrentLevelIndex);

	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading Level: %s"), *LevelMapNames[CurrentLevelIndex].ToString());
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}



void ANum6GameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ANum6PlayerController* Num6PlayerController = Cast<ANum6PlayerController>(PlayerController))
		{
			Num6PlayerController->SetPause(true);
			Num6PlayerController->ShowMainMenu(true);
		}
	}
	
}

void ANum6GameState::UpdateHud()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ANum6PlayerController* Num6PlayerController = Cast<ANum6PlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = Num6PlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"),RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{

					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UNum6GameInstance* Num6GameInstance = Cast<UNum6GameInstance>(GameInstance);
						if (Num6GameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Num6GameInstance->TotalScore)));
						}
					}
				}
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UNum6GameInstance* Num6GameInstance = Cast<UNum6GameInstance>(GameInstance);
						if (Num6GameInstance)
						{
							LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), Num6GameInstance->CurrentLevelIndex + 1)));
						}
					}
				}

			}
		}
	}
}

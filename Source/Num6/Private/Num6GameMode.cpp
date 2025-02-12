#include "Num6GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Num6Character.h"
#include "SpawnVolume.h"
#include "Num6GameState.h"
#include "Num6PlayerController.h"

ANum6GameMode::ANum6GameMode()
{
    DefaultPawnClass = ANum6Character::StaticClass();
    PlayerControllerClass = ANum6PlayerController::StaticClass();
    GameStateClass = ANum6GameState::StaticClass();
}

void ANum6GameMode::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("GameMode BeginPlay 실행됨")); // 로그 추가

    FindSpawnVolumes(); // 스폰 볼륨 찾기
}

void ANum6GameMode::FindSpawnVolumes()
{
    UE_LOG(LogTemp, Warning, TEXT("FindSpawnVolumes 실행됨")); // 로그 추가

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(Actor))
        {
            SpawnVolumes.Add(SpawnVolume);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("감지된 SpawnVolume 개수: %d"), SpawnVolumes.Num());
}



void ANum6GameMode::SpawnItemsEvenly()
{
    if (SpawnVolumes.Num() == 0 || TotalItemsToSpawn <= 0) return;

    int32 BaseSpawnCount = TotalItemsToSpawn / SpawnVolumes.Num(); // 각 볼륨이 가져갈 기본 개수
    int32 ExtraItems = TotalItemsToSpawn % SpawnVolumes.Num(); // 나머지 아이템 개수

    int32 SpawnedItems = 0; // 실제 스폰된 아이템 개수

    for (int32 i = 0; i < SpawnVolumes.Num(); i++)
    {
        if (SpawnedItems >= TotalItemsToSpawn) break; // 총 개수보다 더 많이 스폰되지 않도록 방지

        int32 NumToSpawn = BaseSpawnCount + (i < ExtraItems ? 1 : 0); // 일부 볼륨에 하나 더 추가

        UE_LOG(LogTemp, Warning, TEXT("SpawnVolume[%d]: %d개의 아이템 스폰"), i, NumToSpawn);

        for (int32 j = 0; j < NumToSpawn; j++)
        {
            if (SpawnedItems >= TotalItemsToSpawn) break; // 총 개수 초과 방지
            SpawnVolumes[i]->SpawnRandomItem();
            SpawnedItems++; // 생성된 아이템 개수 증가
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("총 스폰된 아이템 개수: %d"), SpawnedItems);
}


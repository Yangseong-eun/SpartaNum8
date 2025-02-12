#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SpawnVolume.h"
#include "Num6GameMode.generated.h"

UCLASS()
class NUM6_API ANum6GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ANum6GameMode();

protected:
	virtual void BeginPlay() override;

private:
	TArray<ASpawnVolume*> SpawnVolumes; // 모든 스폰 볼륨을 저장할 배열
	int32 TotalItemsToSpawn = 15; // 전체 볼륨에서 스폰할 총 아이템 개수

	void FindSpawnVolumes(); // 모든 스폰 볼륨 찾기
	void SpawnItemsEvenly(); // 모든 스폰 볼륨에서 총 개수를 나누어 스폰
};

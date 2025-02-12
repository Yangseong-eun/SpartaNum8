// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AMineItem::AMineItem()
{
	ItemType = "Mine";
	ExplosionDelay = 5.0f;
	ExplosionRadius= 250.f;
	ExplosionDamage = 30.0f;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);

}

void AMineItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	//Unreal Engine의 SetTimer 함수는 지정된 시간이 지나면 특정 함수를 실행하도록 예약하는 역할을 함

	//GetWorld()->GetTimerManager().SetTimer(
	//	ExplosionTimerHandle,  // 1️⃣ 타이머 핸들
	//	this,                  // 2️⃣ 실행할 객체
	//	&AMineItem::Explode,   // 3️⃣ 실행할 함수
	//	ExplosionDelay,        // 4️⃣ 실행까지 대기할 시간(초 단위)
	//	false                  // 5️⃣ 반복 여부 (false = 한 번만 실행)
	//);


	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::Explode, ExplosionDelay, false);
}

void AMineItem::Explode()
{
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player damage %d by mineItem"), ExplosionDamage));

			// 🎯 데미지 적용
			UGameplayStatics::ApplyDamage(Actor, ExplosionDamage, nullptr, this, UDamageType::StaticClass());
		}
	}

	DestroyItem();
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"
#include "Engine/World.h"
#include "Num6GameState.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
   

    if (Activator && Activator->ActorHasTag("Player"))
    {
      

        if (UWorld* World = GetWorld())
        {
            if (ANum6GameState* GameState = World->GetGameState<ANum6GameState>())
            {
                
                GameState->AddScore(PointValue);

                GameState->OnCoinCollected();
            }
        }
        DestroyItem();
    }
}


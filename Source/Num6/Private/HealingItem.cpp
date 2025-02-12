// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"
#include "Num6Character.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);

    if (Activator && Activator->ActorHasTag("Player"))
    {
        ANum6Character* PlayerCharacter = Cast<ANum6Character>(Activator);
        if (PlayerCharacter)
        {
            UE_LOG(LogTemp, Warning, TEXT("Healing Item Activated! Adding Health..."));

            PlayerCharacter->AddHealth(HealAmount);

            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player gained %d HP"), HealAmount));

            DestroyItem();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Activator is NOT an ANum6Character!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Activator is NULL or doesn't have the Player tag!"));
    }
}



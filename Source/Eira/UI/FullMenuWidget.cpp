// Fill out your copyright notice in the Description page of Project Settings.


#include "FullMenuWidget.h"

#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Player/EiraCharacter.h"

void UFullMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(const AEiraCharacter* PlayerCharacter = Cast<AEiraCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		InventoryComponent = Cast<UInventoryComponent>(PlayerCharacter->GetComponentByClass(UInventoryComponent::StaticClass()));
		if(InventoryComponent)
		{
			InventoryComponent->UpdateInventory.AddUniqueDynamic(this, &UFullMenuWidget::UpdateInventory);
		}	
	}
}

void UFullMenuWidget::UpdateInventory_Implementation(const TArray<FInventoryEntry>& Inventory)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
}

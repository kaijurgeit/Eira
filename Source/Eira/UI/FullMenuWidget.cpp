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
			InventoryComponent->UpdateInventory.AddUniqueDynamic(this, &ThisClass::UpdateInventory);
		}	
	}
}

void UFullMenuWidget::UpdateInventory(FInventoryEntry InventoryEntry)
{
	UE_LOG(LogTemp, Warning, TEXT("%s -> %i : %s"), *FString(__FUNCTION__), InventoryEntry.Count, *InventoryEntry.ItemDef->GetName());
}

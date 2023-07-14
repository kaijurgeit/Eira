// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenu.h"

#include "InventoryMenuSlot.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Player/EiraCharacter.h"

void UInventoryMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if(const AEiraCharacter* PlayerCharacter = Cast<AEiraCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		InventoryComponent = Cast<UInventoryComponent>(PlayerCharacter->GetComponentByClass(UInventoryComponent::StaticClass()));
		if(InventoryComponent)
		{
			InventoryComponent->UpdateInventory.AddUniqueDynamic(this, &UInventoryMenu::UpdateInventory);
		}	
	}
}

void UInventoryMenu::UpdateInventory_Implementation(const TArray<FInventoryEntry>& Inventory)
{
	GridResources->ClearChildren();
}


UGridPanel* UInventoryMenu::SelectGridPanel(const UInventoryFragment_InventoryEntryLayout* Layout)
{
	switch (Layout->Group)
	{
	case EInventoryGroup::Resources:
		return GridResources;
	case EInventoryGroup::Weapons:
		return GridWeapons;
	case EInventoryGroup::Props:
		return GridProps;
	case EInventoryGroup::Craft:
		return GridCraft;
	default:
		return nullptr;
	}
}

int UInventoryMenu::CreateInventorySlots(const FInventoryEntry& Entry, int32 ColCount, int32 StartIndex)
{
	int ColIndex = StartIndex;
	const UInventoryFragment_InventoryEntryLayout* Layout = Entry.ItemDef->FindFragmentByClass<UInventoryFragment_InventoryEntryLayout>();
	if(!Layout) { return ColIndex; }

	const int32 RestCount = Entry.Count % Layout->MaxItemsPerStack;
	int32 StackCount = Entry.Count / Layout->MaxItemsPerStack;
	StackCount = (RestCount == 0) ? StackCount : StackCount + 1;

	
	UGridPanel* GridPanel = SelectGridPanel(Layout);

	for (int i = 0; i < StackCount; ++i)
	{
		UInventoryMenuSlot* InventorySlot = Cast<UInventoryMenuSlot>(CreateWidget(this, SlotClass));
		InventorySlot->ItemDef = Entry.ItemDef;
		const int32 Count = (i < (StackCount - 1) || (RestCount == 0)) ? Layout->MaxItemsPerStack : RestCount;
		InventorySlot->UpdateCount(Count, Layout->MaxItemsPerStack);
		InventorySlot->UpdateIcon(Count, Layout->IconTexture);
	
		ColIndex += i;
		GridPanel->AddChildToGrid(InventorySlot, ColIndex / ColCount, ColIndex % ColCount);
	}
	return ColIndex;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "FullMenuWidget.h"

#include "InventorySlot.h"
#include "Components/GridPanel.h"
#include "Components/TextBlock.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Player/EiraCharacter.h"

UE_DISABLE_OPTIMIZATION

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
	GridResources->ClearChildren();
}


int UFullMenuWidget::CreateInventorySlots(const FInventoryEntry& Entry, int32 ColCount, int32 StartIndex)
{
	int ColIndex = StartIndex;
	const UInventoryFragment_InventoryEntryLayout* Layout = Entry.ItemDef->FindFragmentByClass<UInventoryFragment_InventoryEntryLayout>();
	if(!Layout) { return ColIndex; }

	const int32 RestCount = Entry.Count % Layout->MaxItemsPerStack;
	int32 StackCount = Entry.Count / Layout->MaxItemsPerStack;
	StackCount = (RestCount == 0) ? StackCount : StackCount + 1;

	for (int i = 0; i < StackCount; ++i)
	{
		UInventorySlot* InventorySlot = Cast<UInventorySlot>(CreateWidget(this, SlotClass));
		InventorySlot->ItemDef = Entry.ItemDef;
		if(UTextBlock* TextBlock = Cast<UTextBlock>(InventorySlot->CountText))
		{
			const int32 Count = (i < (StackCount - 1) || (RestCount == 0)) ? Layout->MaxItemsPerStack : RestCount;
			FString String = FString::Printf(TEXT("%d/%d"), Count, Layout->MaxItemsPerStack);
			const FText Text = FText::FromString(String);
			TextBlock->SetText(Text);
			InventorySlot->Count = Count;
		}
		ColIndex += i;
		GridResources->AddChildToGrid(InventorySlot, ColIndex / ColCount, ColIndex % ColCount);
	}
	return ColIndex;
}
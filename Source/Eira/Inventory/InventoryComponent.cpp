// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "InventoryItemDefinition.h"

UE_DISABLE_OPTIMIZATION


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// FInventoryEntry& UInventoryComponent::GetExistingEntry(FInventoryEntry PickupEntry)
// {
// 	FInventoryEntry* InventoryEntry = Entries.FindByPredicate(
// 		[PickupEntry](const FInventoryEntry& Entry)
// 		{
// 			return Entry.ItemDef == PickupEntry.ItemDef;
// 		});
// 	
// 	if(!InventoryEntry)
// 	{
// 		
// 	}
// }

FInventoryEntry* UInventoryComponent::GetOrCreateEntry(UInventoryItemDefinition* PickupItem, int32 FreeStacksInGroup)
{
	FInventoryEntry* InventoryEntry = Entries.FindByPredicate(
		[PickupItem](const FInventoryEntry& Entry)
		{
			return Entry.ItemDef->StaticClass() == PickupItem->StaticClass();
		});

	if(!InventoryEntry)
	{
		if(FreeStacksInGroup < 1)
		{
			return nullptr;
		}
		
		InventoryEntry = &Entries.AddDefaulted_GetRef();
	}
	return InventoryEntry;
}

void UInventoryComponent::AddItemDefinition(FInventoryEntry PickupEntry)
{
	const UUInventoryFragment_InventoryEntryLayout* EntryLayout = Cast<UUInventoryFragment_InventoryEntryLayout>(
		PickupEntry.ItemDef->FindFragmentByClass(UUInventoryFragment_InventoryEntryLayout::StaticClass()));

	if(!EntryLayout)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -> %s"), *FString(__FUNCTION__), *FString("Inventory Full"));
		return;
	}

	FInventoryEntry* InventoryEntry = GetOrCreateEntry(PickupEntry.ItemDef, FreeStacks[EntryLayout->InventoryGroup]);
	
	if(!InventoryEntry) { return; }

	// How much space is free in the partially filled Stack?
	const int32 StackCountFree = InventoryEntry->Count % EntryLayout->StackCountMax;
			
	// How much space is left in total considering the partially filled Stack and all empty Stacks in the Group?
	int32 TotalCountFree = FreeStacks[EntryLayout->InventoryGroup] * EntryLayout->StackCountMax + StackCountFree;
	TotalCountFree = (EntryLayout->TotalCountMax > 0) ? FMath::Min(TotalCountFree, EntryLayout->TotalCountMax) : TotalCountFree;

	// Add to Inventory
	const int32 OldItemCount = InventoryEntry->Count;
	const int32 TotalItemCount = InventoryEntry->Count + PickupEntry.Count;
	InventoryEntry->Count = (TotalItemCount > TotalCountFree) ? TotalCountFree : TotalItemCount;

	// Update Free Stacks
	FreeStacks[EntryLayout->InventoryGroup] = FreeStacks[EntryLayout->InventoryGroup] - (InventoryEntry->Count - OldItemCount) / EntryLayout->StackCountMax;

	// How many items did not fit into the inventory?
	int32 RestCount = FMath::Max(0, TotalItemCount - TotalCountFree);
	
	UE_LOG(LogTemp, Warning, TEXT("%s -> Count: %i; Free Stacks: %i; Rest: %i"), *FString(__FUNCTION__), InventoryEntry->Count, FreeStacks[EntryLayout->InventoryGroup], RestCount);
}


